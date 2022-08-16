#include "DecodeAudio.h"

/**
 * @brief When audio data is present in the `audioFrame`,
 *        emit the audio data to the `audioCallback`.
 */
int FFmpeg_emit_audio_frame(FFmpegContext *context, value emitAudioCallback)
{
    // context->audioFrameQueue contains the latest audio frame.
    // printf("[extension-ffmpegA] Emitting audio frame...\n");

    // Resample the audio frame before emitting.
    // printf("[extension-ffmpegA] Attempting software resampling...\n");
    AVFrame *audioFrame = FFmpegFrameQueue_pop(context->audioFrameQueue);
    int result = FFmpegContext_swr_resample_audio_frame(context, audioFrame);
    if (result < 0)
    {
        // Failed to convert the audio frame.
        // printf("[extension-ffmpegA] Failed to convert audio frame.\n");
        return result;
    }

    // Since we are in a thread, accessing the buffer and executing the callback
    // must be performed as a safe operation.
    cffi_safe_operation([context, emitAudioCallback]() {
        // printf("[extension-ffmpegA] Allocating buffer (audio frame %d)...\n", context->audioOutputFrameSize);
        buffer out_buffer = cffi_build_buffer(context->audioOutputFrameBuffer, context->audioOutputFrameSize);

        // printf("[extension-ffmpegA] Emitting audio frame via callback...\n");
        value out_buffer_val = buffer_val(out_buffer);
        val_call1(emitAudioCallback, out_buffer_val);
        // printf("[extension-ffmpegA] Emitted audio frame to callback...\n");
    });

    // printf("[extension-ffmpegA] Success.\n");

    return 0;
}

/**
 * @brief Continuously emits frames from the audio stream.
 *        Designed to be run in a separate thread.
 */
void FFmpeg_audio_thread(FFmpegContext *context, value emitAudioCallback)
{
    // printf("[extension-ffmpeg] Audio thread started.\n");
    while (!context->quit)
    {
        int result = FFmpeg_emit_audio_frame(context, emitAudioCallback);
        if (result < 0)
        {
            // Failed to decode the frame.
            // printf("[extension-ffmpeg] Failed to decode audio frame: %d\n", result);
            break;
        }
    }
    // printf("[extension-ffmpeg] Decode thread finished.\n");
}

/**
 * @brief Attempt to place `outputLen` bytes of audio data into `outputData`.
 *        Will place silence if not enough data is available.
 */
int FFmpeg_generate_audio(FFmpegContext *context, char *outputData, int outputLen) {
    int dataWritten = 0;

    // printf("[extension-ffmpeg] Attempting to generate audio...\n");
    while(dataWritten < outputLen) {
        int spaceAvailable = outputLen - dataWritten;

        if (context->audioOutputLeftoverBufferSize > 0) {
            int dataToWrite = fmin(spaceAvailable, context->audioOutputLeftoverBufferSize);
            // printf("[extension-ffmpeg] Writing %d bytes FROM BUFFER...\n", dataToWrite);
            memcpy(outputData + dataWritten, context->audioOutputLeftoverBuffer, dataToWrite);

            if (dataToWrite < context->audioOutputLeftoverBufferSize) {
                // printf("[extension-ffmpeg] Shifting leftover buffer...\n");
                context->audioOutputLeftoverBuffer += dataToWrite;
                context->audioOutputLeftoverBufferSize -= dataToWrite;
            } else {
                // printf("[extension-ffmpeg] Clearing leftover buffer...\n");
                context->audioOutputLeftoverBuffer = nullptr;
                context->audioOutputLeftoverBufferSize = 0;
            }

            continue;
        }

        // Else, buffer is empty and we can write from the next frame.
        AVFrame* audioFrame = NULL;
        if (dataWritten > 0) {
            audioFrame = FFmpegFrameQueue_tryPop(context->audioFrameQueue);
        } else {
            audioFrame = FFmpegFrameQueue_pop(context->audioFrameQueue);
        }

        if (audioFrame != NULL) {
            // printf("[extension-ffmpeg] Got audio frame.\n");
            int result = FFmpegContext_swr_resample_audio_frame(context, audioFrame);
            if (result < 0) {
                // Failed to convert the audio frame.
                printf("[extension-ffmpeg] Failed to convert audio frame.\n");
                return result;
            }

            int dataToWrite = fmin(spaceAvailable, context->audioOutputFrameSize);

            // printf("[extension-ffmpeg] Writing %d bytes...\n", context->audioOutputFrameSize);
            memcpy(outputData + dataWritten, context->audioOutputFrameBuffer, dataToWrite);
            dataWritten += dataToWrite;

            if (dataToWrite < context->audioOutputFrameSize) {
                int dataLeftover = context->audioOutputFrameSize - dataToWrite;
                // Hold onto the leftover data.
                // printf("[extension-ffmpeg] Reserving %d bytes of leftover audio data...\n", dataLeftover);
                context->audioOutputLeftoverBuffer = context->audioOutputFrameBuffer + dataToWrite;
                context->audioOutputLeftoverBufferSize = dataLeftover;
            }
        } else {
            //printf("[extension-ffmpeg] Writing %d bytes of silence...\n", spaceAvailable);
            printf("[extension-ffmpeg] Queue status: V%d / A%d\n", context->videoFrameQueue->size, context->audioFrameQueue->size);
            memset(outputData + dataWritten, 0, spaceAvailable);
            dataWritten += spaceAvailable;
        }
    }

    //printf("[extension-ffmpeg] Wrote %d bytes.\n", dataWritten);

    return 0;
}

DEFINE_FUNC_3(hx_ffmpeg_generate_audio, context, audioBufferValue, outputLen) {
    // printf("[extension-ffmpeg] Generating audio...\n");

    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);

    buffer audioBuffer = val_to_buffer(audioBufferValue);
	char *audioBufferPtr = buffer_data(audioBuffer);
	int audioBufferLen = buffer_size(audioBuffer);

    int outputLenInt = val_int(outputLen);

    if (outputLenInt > audioBufferLen) {
        printf("[extension-ffmpeg] Output buffer is too small.\n");
        return alloc_int(-1);
    }

    // printf("[extension-ffmpeg] Generating %d bytes of audio...\n", outputLenInt);

    int result = FFmpeg_generate_audio(contextPointer, audioBufferPtr, outputLenInt);
    return alloc_int(result);
}

/**
 * @brief Starts a thread to emit frames from the audio stream.
 */
DEFINE_FUNC_2(hx_ffmpeg_start_audio_thread, context, emitAudioCallback)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    contextPointer->audioThread = new std::thread(FFmpeg_audio_thread, contextPointer, emitAudioCallback);

    return alloc_int(0);
}

/**
 * @brief Stops the audio thread.
 */
DEFINE_FUNC_1(hx_ffmpeg_stop_audio_thread, context)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    contextPointer->quit = true;
    contextPointer->audioThread->join();
    delete contextPointer->audioThread;

    return alloc_int(0);
}
