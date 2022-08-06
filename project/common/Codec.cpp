#include "Codec.h"

int FFmpegContext_avcodec_init_video_codec(FFmpegContext *context)
{
    // Initialize the video codec context.
    context->videoCodecCtx = avcodec_alloc_context3(context->videoCodec);

    if (context->videoCodecCtx == nullptr)
    {
        printf("[extension-ffmpeg] Failed to allocate video codec context.\n");
        return -1;
    }

    // Set parameters of the codec context, based on the stream
    int result = avcodec_parameters_to_context(
        context->videoCodecCtx,
        context->videoStream->codecpar);

    if (result < 0)
    {
        // Failed to set parameters on codec context.
        printf("[extension-ffmpeg] Failed to set parameters on video codec context.\n");
        return result;
    }

    // Initialize the video codec.
    result = avcodec_open2(context->videoCodecCtx, context->videoCodec, NULL);

    if (result < 0)
    {
        // Failed to open the video codec.
        printf("[extension-ffmpeg] Failed to open video codec.\n");
        return result;
    }

    // Success.
    printf("[extension-ffmpeg] Successfully initialized video codec.\n");

    // Perform additional initialization.
    result = FFmpegContext_init_swsCtx(context);
    if (result < 0)
    {
        // Failed to initialize the SWS context.
        printf("[extension-ffmpeg] Failed to initialize the SWS context.\n");
        return result;
    }

    result = FFmpegContext_allocate_video_frame(context);
    if (result < 0)
    {
        // Failed to allocate the video frame.
        printf("[extension-ffmpeg] Failed to allocate the video frame.\n");
        return result;
    }

    return 0;
}

int FFmpegContext_allocate_video_frame(FFmpegContext *context)
{
    printf("Allocating output frame...\n");
    // Determine the required buffer size and allocate it.
    context->videoFrameRGB->format = FFMPEG_PIXEL_FORMAT;
    context->videoFrameRGB->width = context->videoCodecCtx->width;
    context->videoFrameRGB->height = context->videoCodecCtx->height;

    context->videoFrameRGBBuffer = (uint8_t*) av_malloc(
        av_image_get_buffer_size(FFMPEG_PIXEL_FORMAT, context->videoFrameRGB->width, context->videoFrameRGB->height, 32)
    );

    int result = av_image_fill_arrays(
        context->videoFrameRGB->data,
        context->videoFrameRGB->linesize,
        context->videoFrameRGBBuffer,
        AV_PIX_FMT_BGRA,
        context->videoFrameRGB->width,
        context->videoFrameRGB->height,
        32
    );

    if (result < 0) {
        // Failed to allocate the image.
        printf("[extension-ffmpeg] Failed to allocate frame buffer.\n");
        return result;
    }

    // Success.
    return 0;
}

/**
 * @brief Initializes the codec for the stream's video output.
 *
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the codec was initialized successfully.
 */
value __hx_ffmpeg_avcodec_init_video_codec(value context)
{
    printf("[extension-ffmpeg] Initializing video codec.\n");
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    int result = FFmpegContext_avcodec_init_video_codec(contextPointer);

    return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_avcodec_init_video_codec, context)
{
    return __hx_ffmpeg_avcodec_init_video_codec(context);
}

int FFmpegContext_avcodec_init_audio_codec(FFmpegContext *context)
{
    // Initialize the audio codec context.
    context->audioCodecCtx = avcodec_alloc_context3(context->audioCodec);

    if (context->audioCodecCtx == nullptr)
    {
        printf("[extension-ffmpeg] Failed to allocate audio codec context.\n");
        return -1;
    }

    // Set parameters of the codec context, based on the stream
    int result = avcodec_parameters_to_context(
        context->audioCodecCtx,
        context->audioStream->codecpar);

    if (result < 0)
    {
        // Failed to set parameters on codec context.
        printf("[extension-ffmpeg] Failed to set parameters on audio codec context.\n");
        return result;
    }

    // Initialize the video codec.
    result = avcodec_open2(context->audioCodecCtx, context->audioCodec, NULL);

    if (result < 0)
    {
        // Failed to open the video codec.
        printf("[extension-ffmpeg] Failed to open audio codec.\n");
        return result;
    }

    // Success.
    printf("[extension-ffmpeg] Successfully initialized audio codec.\n");

    // Perform additional initialization.
    result = FFmpegContext_init_swrCtx(context);
    if (result < 0)
    {
        // Failed to initialize the SWS context.
        printf("[extension-ffmpeg] Failed to initialize the SWR context.\n");
        return result;
    }

    return 0;
}

/**
 * @brief Initializes the codec for the stream's audio output.
 *
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the codec was initialized successfully.
 */
value __hx_ffmpeg_avcodec_init_audio_codec(value context)
{
    printf("[extension-ffmpeg] Initializing audio codec.\n");
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    int result = FFmpegContext_avcodec_init_audio_codec(contextPointer);

    return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_avcodec_init_audio_codec, context)
{
    return __hx_ffmpeg_avcodec_init_audio_codec(context);
}


/**
 * @brief Returns the height of the current video stream.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return The height of the video stream in pixels.
 */
value __hx_ffmpeg_get_video_frame_number(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  
  int result = contextPointer->videoCodecCtx->frame_number;
  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_get_video_frame_number, context)
{
  return __hx_ffmpeg_get_video_frame_number(context);
}