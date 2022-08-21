#include "DecodeVideo.h"

int FFmpeg_emit_video_frame(FFmpegContext *context, value emitVideoCallback)
{
    // context->videoFrameQueue contains the next few video frames.
    // printf("[extension-ffmpegV] Running video thread...\n");

    // Convert the video frame from YUV (or whatever it is) to an RGB frame.
    // printf("[extension-ffmpegV] Start software scaling...\n");
    AVFrame* videoFrame = FFmpegFrameQueue_pop(context->videoFrameQueue);
    if (videoFrame == nullptr)
    {
        printf("[extension-ffmpegV] No video frame available. You may see a stutter.\n");
        return -1;
    }

    int result = FFmpegContext_sws_scale_video_frame(context, videoFrame);

    if (result < 0)
    {
        // Failed to convert the video frame.
        // printf("[extension-ffmpegV] Failed to convert video frame.\n");
        return result;
    }

    if (!val_is_null(emitVideoCallback)) {
        printf("[extension-ffmpegV] Emitting video frame to callback...\n");

        // Since we are in a thread, allocating the buffer and executing the callback
        // must be performed as a safe operation.
        cffi_safe_operation([context, emitVideoCallback]()
                            {
            // printf("[extension-ffmpegV] Allocating buffer (buffer %d)...\n", context->videoOutputFrameSize);
            buffer out_buffer = cffi_build_buffer(context->videoOutputFrameBuffer, context->videoOutputFrameSize);

            // printf("[extension-ffmpegV] Emitting video frame via callback...\n");
            value out_buffer_val = buffer_val(out_buffer);
            val_call1(emitVideoCallback, out_buffer_val);
            // printf("[extension-ffmpegV] Emitted video frame to callback...\n"); });
        });
    } else {
        // printf("Nowhere to emit frame to!");
    }

    return 0;
}

double FFmpeg_sync_video_pts(FFmpegContext *context, AVFrame* avFrame)
{
    double framePTS = avFrame->pts;

    if (framePTS != 0 && framePTS != AV_NOPTS_VALUE) {
        // If we have an up-to-date PTS, update the clock.
        context->videoClock = framePTS;
    } else {
        // If we don't have a valid PTS, then set it to the current time.
        framePTS = context->videoClock;
    }
    
    // Update the time to use for the next frame.
    double frameDelay = context->videoTimeBase;
    /* if we are repeating a frame, adjust clock accordingly */
    frameDelay += avFrame->repeat_pict * (context->videoTimeBase * 0.5);
    context->videoClock += frameDelay;

    return framePTS;
}

/**
 * @brief Continuously emits frames from the video stream.
 *        Designed to be run in a separate thread.
 */
void FFmpeg_video_thread(FFmpegContext *context, value emitVideoCallback)
{
    // printf("[extension-ffmpeg] Video thread started.\n");
    while (!context->quit)
    {
        double delayPTS = context->videoOutputFrame->pts - context->videoLastFramePts;
        double delaySeconds = delayPTS * context->videoTimeBase;

        if (delaySeconds <= 0.0 || delaySeconds >= 1.0) {
            // Use the previous delay if the current one is invalid.
            delaySeconds = context->videoLastFramePts * context->videoTimeBase;
        }

        context->videoLastFramePts = context->videoOutputFrame->pts;
        context->videoLastFrameDelay = delayPTS;

        // Sync with audio output.
        double audioClock = context->audioClock;
        double diffPTS = context->videoOutputFrame->pts - audioClock;
        double diffSeconds = diffPTS * context->videoTimeBase;

        double syncThreshold = (diffSeconds > FFMPEG_SYNC_THRESHOLD) ? diffSeconds : FFMPEG_SYNC_THRESHOLD;

        // Based on audio delay, skip or repeat the current frame.
        if (fabs(diffSeconds) < FFMPEG_NOSYNC_THRESHOLD) {
            if(diffSeconds <= -syncThreshold) {
                // Immediately display the next frame.
	            delaySeconds = 0;
	        } else if(diffSeconds >= syncThreshold) {
                // Wait to display the next frame.
	            delaySeconds = 2 * delaySeconds;
	        }
        }

        context->videoTimer += delaySeconds;
        double realDelay = context->videoTimer - (av_gettime() / 1000000.0);

        // Limit to 100 FPS.
        if (realDelay < 0.01) {
            realDelay = 0.01;
        }

        // WAIT for the next frame.
        av_usleep((unsigned int)(realDelay * 1000000.0));

        // Then we display it.
        int result = FFmpeg_emit_video_frame(context, emitVideoCallback);
        if (result < 0)
        {
            // Failed to decode the frame.
            // printf("[extension-ffmpeg] Failed to decode video frame: %d\n", result);
            break;
        }
    }
    // printf("[extension-ffmpeg] Decode thread finished.\n");
}

/**
 * @brief Starts a thread to emit frames from the video stream.
 */
DEFINE_FUNC_2(hx_ffmpeg_start_video_thread, context, emitVideoCallback)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    contextPointer->videoThread = new std::thread(FFmpeg_video_thread, contextPointer, emitVideoCallback);

    return alloc_int(0);
}

/**
 * @brief Stops the video thread.
 */
DEFINE_FUNC_1(hx_ffmpeg_stop_video_thread, context)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    contextPointer->quit = true;
    contextPointer->videoThread->join();
    delete contextPointer->videoThread;

    return alloc_int(0);
}

/**
 * @brief Returns the current frame number of the video stream.
 *
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return The current frame number of the video stream.
 */
DEFINE_FUNC_1(hx_ffmpeg_get_video_frame_number, context)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);

    int result = contextPointer->videoCodecCtx->frame_number;
    return alloc_int(result);
}

/**
 * @brief Attempt to place the latest frame of video data into `outputData`.
 */
int FFmpeg_generate_video(FFmpegContext *context, buffer videoBuffer) {
    size_t bufferLength = buffer_size(videoBuffer);

    if (bufferLength < context->videoOutputFrameSize) {
        buffer_set_size(videoBuffer, context->videoOutputFrameSize);
    }

    char* bufferData = buffer_data(videoBuffer);

    memcpy(bufferData, context->videoOutputFrameBuffer, context->videoOutputFrameSize);
    
    return 0;
}

DEFINE_FUNC_2(hx_ffmpeg_generate_video, context, videoBufferValue) {
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);

    buffer videoBuffer = val_to_buffer(videoBufferValue);

    // printf("[extension-ffmpeg] Generating %d bytes of audio...\n", outputLenInt);

    int result = FFmpeg_generate_video(contextPointer, videoBuffer);
    return alloc_int(result);
}