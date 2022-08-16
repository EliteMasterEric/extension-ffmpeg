#include "SoftwareResample.h"

int FFmpegContext_init_swrCtx(FFmpegContext *context)
{
    // TODO: Change the destionation width and height to the size provided by the user,
    // and see if performance/memory usage improves.

    // Allocate software resampler context and set parameters.
    int result = swr_alloc_set_opts2(
            &context->swrCtx,
            &context->audioOutputChannelLayout, AV_SAMPLE_FMT_FLT, FFMPEG_BITRATE,
            &context->audioCodecCtx->ch_layout, context->audioCodecCtx->sample_fmt, context->audioCodecCtx->sample_rate,
    		0, NULL);

    if (result < 0) {
        // printf("[extension-ffmpeg] Failed to allocate software resampler context.\n");
        return result;
    }

	result = swr_init(context->swrCtx);
    if (result < 0) {
        // printf("[extension-ffmpeg] Failed to initialize the software resampler context.\n");
        return result;
    }

    return 0;
}

int FFmpegContext_swr_resample_audio_frame(FFmpegContext *context, AVFrame* audioFrame) {
    if (context->swrCtx == nullptr) {
        // printf("[extension-ffmpeg] Software resampler not initialized.\n");
        return -1;
    }

    int in_rate = context->audioCodecCtx->sample_rate;
    int in_samples = audioFrame->nb_samples;
    int out_samples = av_rescale_rnd(swr_get_delay(context->swrCtx, in_rate) + in_samples, FFMPEG_BITRATE, in_rate, AV_ROUND_UP);
    AVChannelLayout out_ch_layout = context->audioOutputChannelLayout;
    int out_channels = out_ch_layout.nb_channels;

    context->audioOutputFrameSize = av_samples_get_buffer_size(NULL, out_channels, out_samples, AV_SAMPLE_FMT_FLT, 1);
    // printf("Allocating output buffer of size %d...\n", context->audioOutputFrameSize);

    context->audioOutputFrameBuffer = (uint8_t*) av_malloc(context->audioOutputFrameSize);

    const uint8_t** in_buffer = (const uint8_t**) audioFrame->data;

    // Takes a number of SAMPLES as an argument, not a number of bytes.
    int result = swr_convert(context->swrCtx, &context->audioOutputFrameBuffer, out_samples, in_buffer, in_samples);

    /* Keep audio_clock up-to-date as we decode packets */
    int pts = context->audioClock;
    int n = 2 * context->audioCodecCtx->channels;
    context->audioClock += (double) context->audioOutputFrameSize / (double)(n * context->audioCodecCtx->sample_rate);

    if (result < 0) {
        // printf("[extension-ffmpeg] Failed to resample the audio frame.\n");
        return result;
    }

    // The contents will now be in the audioOutputFrameBuffer.
    return 0;
}