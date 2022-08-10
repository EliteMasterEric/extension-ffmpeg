#include "DecodeVideo.h"

// Emitting video frames is manual, while emitting audio frames is automatic.

int FFmpeg_emit_video_frame(FFmpegContext *context, value emitVideoCallback) {
    // context->videoFrameQueue contains the next few video frames.

    // Convert the video frame from YUV (or whatever it is) to an RGB frame.
    int result = FFmpegContext_sws_scale_video_frame(context);

    if (result < 0) {
        // Failed to convert the video frame.
        printf("[extension-ffmpeg] Failed to convert video frame.\n");
        return result;
    }

    // Write the newly converted video frame to the output buffer.
    int data_length = av_image_get_buffer_size(FFMPEG_PIXEL_FORMAT, context->videoOutputFrame->width, context->videoOutputFrame->height, 32);
    
    buffer out_buffer = cffi_build_buffer(context->videoOutputFrameBuffer, context->videoOutputFrameSize);    

    // Call the emitVideoCallback function.
    val_call1(emitVideoCallback, buffer_val(out_buffer));

    if (result < 0) {
        // Failed to copy the video frame.
        printf("[extension-ffmpeg] Failed to copy video frame.\n");
        return result;
    }

    return 0;
}

/**
 * @brief Continuously emits frames from the video stream.
 *        Designed to be run in a separate thread.
 */
void FFmpeg_video_thread(FFmpegContext *context, value emitVideoCallback)
{
    printf("[extension-ffmpeg] Video thread started.\n");
    while (!context->quit)
    {
        int result = FFmpeg_emit_video_frame(context, emitVideoCallback);
        if (result < 0)
        {
            // Failed to decode the frame.
            printf("[extension-ffmpeg] Failed to decode video frame: %d\n", result);
            break;
        }
    }
    printf("[extension-ffmpeg] Decode thread finished.\n");
}

/**
 * @brief Starts a thread to emit frames from the video stream.
 */
DEFINE_FUNC_2(hx_ffmpeg_start_video_thread, context, emitVideoCallback)
{
    FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
    contextPointer->videoThread = new std::thread(FFmpeg_video_thread, contextPointer, emitVideoCallback);

    return alloc_int(0);
}

/**
 * @brief Stops the video thread.
 */
DEFINE_FUNC_1(hx_ffmpeg_stop_video_thread, context)
{
    FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
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
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  
  int result = contextPointer->videoCodecCtx->frame_number;
  return alloc_int(result);
}