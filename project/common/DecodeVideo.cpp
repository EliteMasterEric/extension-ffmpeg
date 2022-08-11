#include "DecodeVideo.h"

int FFmpeg_emit_video_frame(FFmpegContext *context, value emitVideoCallback)
{
    // context->videoFrameQueue contains the next few video frames.
    // printf("[extension-ffmpegV] Running video thread...\n");

    // Convert the video frame from YUV (or whatever it is) to an RGB frame.
    // printf("[extension-ffmpegV] Start software scaling...\n");
    int result = FFmpegContext_sws_scale_video_frame(context);

    if (result < 0)
    {
        // Failed to convert the video frame.
        // printf("[extension-ffmpegV] Failed to convert video frame.\n");
        return result;
    }

    // Since we are in a thread, allocating the buffer and executing the callback
    // must be performed as a safe operation.
    cffi_safe_operation([context, emitVideoCallback]()
                        {
        // printf("[extension-ffmpegV] Allocating buffer (buffer %d)...\n", context->videoOutputFrameSize);
        buffer out_buffer = cffi_build_buffer(context->videoOutputFrameBuffer, context->videoOutputFrameSize);

        // printf("[extension-ffmpegV] Emitting video frame via callback...\n");
        value out_buffer_val = buffer_val(out_buffer);
        val_call1(emitVideoCallback, out_buffer_val);
        // printf("[extension-ffmpegV] Emitted video frame to callback...\n");
    });

    if (result < 0)
    {
        // Failed to copy the video frame.
        // printf("[extension-ffmpegV] Failed to copy video frame.\n");
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
    // printf("[extension-ffmpeg] Video thread started.\n");
    while (!context->quit)
    {
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