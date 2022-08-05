#include "DecodeVideo.h"

int FFmpeg_emit_video_frame(FFmpegContext *context, value callback) {
    // context->videoFrame contains the latest video frame.

    // Convert the video frame to an RGB frame.
    sws_scale(context->swsCtx,
        context->videoFrame->data, context->videoFrame->linesize, 0,
        context->videoCodecCtx->height, context->videoFrameRGB->data, context->videoFrameRGB->linesize);

    // Create a new Haxe object to wrap the RGB frame.
    int data_length = av_image_get_buffer_size(FFMPEG_PIXEL_FORMAT, context->videoFrameRGB->width, context->videoFrameRGB->height, 32);

    // Copy the RGB frame into the new Haxe buffer.
    buffer out_buffer = alloc_buffer(NULL);
    buffer_append_sub(out_buffer, (const char*) context->videoFrameRGB->data[0], data_length);

    // Execute the callback, passing the latest frame as the argument.
    val_call1(callback, buffer_val(out_buffer));

    return 0;
}