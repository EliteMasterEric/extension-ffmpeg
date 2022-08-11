#include "SoftwareScale.h"

int FFmpegContext_init_swsCtx(FFmpegContext *context)
{
    // printf("[extension-ffmpeg] Initializing software scaling context.\n");
    context->swsCtx = sws_getContext(
        // Source
        context->videoCodecCtx->width, context->videoCodecCtx->height, context->videoCodecCtx->pix_fmt,
        // Destination
        context->videoCodecCtx->width, context->videoCodecCtx->height, FFMPEG_PIXEL_FORMAT,
        // Flags
        SWS_BILINEAR, NULL, NULL, NULL);

    if (context->swsCtx == nullptr) {
        // printf("[extension-ffmpeg] Failed to initialize the SWS context.\n");
        return -1;
    }

    return 0;
}

int FFmpegContext_sws_scale_video_frame(FFmpegContext *context) {
    if (context->swsCtx == nullptr) {
        // printf("[extension-ffmpegV] Software scaler not initialized.\n");
        return -1;
    }

    // printf("[extension-ffmpegV] Begin software scaling...\n");

    // printf("[extension-ffmpegV] Attempting to pop video frame...\n");
    AVFrame* videoFrame = FFmpegFrameQueue_pop(context->videoFrameQueue);

    // printf("[extension-ffmpegV] Performing scaling...\n");
    int result = sws_scale(context->swsCtx,
        videoFrame->data, videoFrame->linesize, 0,
        context->videoCodecCtx->height, context->videoOutputFrame->data, context->videoOutputFrame->linesize);

    if (result < 0) {
        // printf("[extension-ffmpegV] Failed to scale the video frame.\n");
        return result;
    }

    return 0;
}