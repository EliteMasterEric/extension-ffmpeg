#include "DecodeAudio.h"

/**
 * @brief When audio data is present in the `audioFrame`,
 *        emit the audio data to the `audioCallback`.
 */
int FFmpeg_emit_audio_frame(FFmpegContext *context, value emitAudioCallback) {
    // context->audioFrameQueue contains the latest video frame.
    printf("[extension-ffmpeg] Emitting audio frame...\n");

    // Resample the audio frame before emitting.
    printf("[extension-ffmpeg] Attempting software resampling...");
    int result = FFmpegContext_swr_resample_audio_frame(context);
    if (result < 0) {
        // Failed to convert the audio frame.
        printf("[extension-ffmpeg] Failed to convert audio frame.\n");
        return result;
    }

    printf("[extension-ffmpeg] Wrapping data for audio callback...");
    value out_buffer = buffer_val(context->audioOutputBuffer);

    printf("[extension-ffmpeg] Executing audio callback...");
    val_call1(emitAudioCallback, out_buffer);

    printf("[extension-ffmpeg] Success.\n");

    return 0;
}

/**
 * @brief Continuously emits frames from the audio stream.
 *        Designed to be run in a separate thread.
 */
void FFmpeg_audio_thread(FFmpegContext *context, value emitAudioCallback)
{
    printf("[extension-ffmpeg] Audio thread started.\n");
    while (!context->quit)
    {
        int result = FFmpeg_emit_audio_frame(context, emitAudioCallback);
        if (result < 0)
        {
            // Failed to decode the frame.
            printf("[extension-ffmpeg] Failed to decode audio frame: %d\n", result);
            break;
        }
    }
    printf("[extension-ffmpeg] Decode thread finished.\n");
}

/**
 * @brief Starts a thread to emit frames from the audio stream.
 */
DEFINE_FUNC_2(hx_ffmpeg_start_audio_thread, context, emitAudioCallback)
{
    FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
    contextPointer->audioThread = new std::thread(FFmpeg_audio_thread, contextPointer, emitAudioCallback);

    return alloc_int(0);
}

/**
 * @brief Stops the audio thread.
 */
DEFINE_FUNC_1(hx_ffmpeg_stop_audio_thread, context)
{
    FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
    contextPointer->quit = true;
    contextPointer->audioThread->join();
    delete contextPointer->audioThread;
    
    return alloc_int(0);
}
