#include "SoftwareScale.h"

int FFmpegContext_init_swsCtx(FFmpegContext *context)
{
    // TODO: Change the destionation width and height to the size provided by the user,
    // and see if performance/memory usage improves.

    printf("Initializing software scaling context.\n");
    context->swsCtx = sws_getContext(
        // Source
        context->videoCodecCtx->width, context->videoCodecCtx->height, context->videoCodecCtx->pix_fmt,
        // Destination
        context->videoCodecCtx->width, context->videoCodecCtx->height, FFMPEG_PIXEL_FORMAT,
        // Flags
        SWS_BILINEAR, NULL, NULL, NULL);

    if (context->swsCtx == nullptr) {
        printf("[extension-ffmpeg] Failed to initialize the SWS context.\n");
        return -1;
    }

    return 0;
}

int FFmpegContext_sws_scale_video_frame(FFmpegContext *context) {
    if (context->swsCtx == nullptr) {
        printf("[extension-ffmpeg] Software scaler not initialized.\n");
        return -1;
    }

    int result = sws_scale(context->swsCtx,
        context->videoFrame->data, context->videoFrame->linesize, 0,
        context->videoCodecCtx->height, context->videoFrameRGB->data, context->videoFrameRGB->linesize);

    if (result < 0) {
        printf("[extension-ffmpeg] Failed to scale the video frame.\n");
        return result;
    }

    return 0;
}