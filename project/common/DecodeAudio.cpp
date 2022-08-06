#include "DecodeAudio.h"

// Emitting audio frames is automatic, while emitting video frames is manual.

int FFmpeg_emit_audio_frame(FFmpegContext *context, value emitAudioCallback) {
    // context->audioFrame contains the latest video frame.
    printf("Emitting audio frame...\n");

    // Convert the audio frame.
    printf("Attempting software resampling...");
    int result = FFmpegContext_swr_resample_audio_frame(context);
    if (result < 0) {
        // Failed to convert the audio frame.
        printf("[extension-ffmpeg] Failed to convert audio frame.\n");
        return result;
    }

    printf("Wrapping data...");
    value out_buffer = buffer_val(context->audioOutputBuffer);

    printf("Executing audio callback...");
    val_call1(emitAudioCallback, out_buffer);

    printf("Success.\n");

    return 0;
}
