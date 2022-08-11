#include "Format.h"

/**
 * @brief Opens an input file, allowing avformat to determine file format information.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @param filename A Haxe object wrapping the name of the file to open.
 * @return 0 if successful, otherwise an error code.
 */
DEFINE_FUNC_2(hx_ffmpeg_avformat_open_input, context, filename)
{
  // printf("Opening input...\n");
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  const char* filenamePointer = val_get_string(filename);
  // printf("%s\n", filenamePointer);
  int result = avformat_open_input(&contextPointer->avFormatCtx, filenamePointer, NULL, NULL);
  // printf("Opened input.\n");

  return alloc_int(result);
}

/**
 * @brief Parses additional information about the media,
 *        such as the frame rate and the available streams.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return 0 if successful, otherwise an error code.
 */
DEFINE_FUNC_1(hx_ffmpeg_avformat_find_stream_info, context)
{
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = avformat_find_stream_info(contextPointer->avFormatCtx, NULL);

  return alloc_int(result);
}

/**
 * @brief Attempt to find the best "video" stream in the file.
 */
int FFmpegContext_av_find_video_stream(FFmpegContext* context) {

  // Identify the "best" video stream.
  const AVCodec* videoCodec = nullptr;
  int videoStreamIndex = av_find_best_stream(context->avFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &videoCodec, 0);

  // If we found a video stream, cache its info.
  if (videoStreamIndex >= 0) {
    // printf("[extension-ffmpeg] Found video stream %d.\n", videoStreamIndex);
    context->videoStreamIndex = videoStreamIndex;
    context->videoStream = context->avFormatCtx->streams[videoStreamIndex];
    context->videoCodec = videoCodec;
  } else {
    // An error occurred.
  }
  return videoStreamIndex;
}

/**
 * @brief Attempt to find the best "audio" stream in the file.
 */
int FFmpegContext_av_find_audio_stream(FFmpegContext* context) {
  // Identify the "best" audio stream.
  const AVCodec* audioCodec = nullptr;
  int audioStreamIndex = av_find_best_stream(context->avFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &audioCodec, 0);

  // If we found an audio stream, cache its info.
  if (audioStreamIndex >= 0) {
    // printf("[extension-ffmpeg] Found audio stream %d.\n", audioStreamIndex);
    context->audioStreamIndex = audioStreamIndex;
    context->audioStream = context->avFormatCtx->streams[audioStreamIndex];
    context->audioCodec = audioCodec;
  } else {
    // An error occurred.
  }
  return audioStreamIndex;
}

/**
 * @brief Attempt to find the best "subtitle" stream in the file.
 */
int FFmpegContext_av_find_subtitle_stream(FFmpegContext* context) {
  // Identify the "best" subtitle stream.
  const AVCodec* subtitleCodec = nullptr;
  int subtitleStreamIndex = av_find_best_stream(context->avFormatCtx, AVMEDIA_TYPE_SUBTITLE, -1, -1, &subtitleCodec, 0);

  // If we found a subtitle stream, cache its info.
  if (subtitleStreamIndex >= 0) {
    // printf("[extension-ffmpeg] Found subtitle stream %d.\n", subtitleStreamIndex);
    context->subtitleStreamIndex = subtitleStreamIndex;
    context->subtitleStream = context->avFormatCtx->streams[subtitleStreamIndex];
    context->subtitleCodec = subtitleCodec;
  } else {
    // An error occurred.
    // printf("[extension-ffmpeg] No subtitle stream\n");
  }
  return subtitleStreamIndex;
}

/**
 * @brief Dumps additional information about the stream to the console.
 * 
 * @param context The FFmpegContext to dump info for.
 */
void FFmpegContext_av_dump_format(FFmpegContext* context) {
  av_dump_format(context->avFormatCtx, 0, "debug.log", 0);

  // Let's dump some other stuff too.
  // printf("========================================================\n");
  // printf("[extension-ffmpeg] Additional information:\n");
  // // printf("Average frame rate: %f\n", av_q2d(context->videoStream->avg_frame_rate));
  // // printf("Timebase: %d/%d\n", context->videoStream->time_base.num, context->videoStream->time_base.den);
  // // printf("Real framerate: %f\n", av_q2d(context->videoStream->r_frame_rate));
  // printf("========================================================\n");
}

/**
 * @brief Dumps additional information about the stream to the console.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to dump info for.
 */
DEFINE_FUNC_1(hx_ffmpeg_av_dump_format, context)
{
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  FFmpegContext_av_dump_format(contextPointer);

  return alloc_null();
}

/**
 * @brief Locates and caches the best video stream in the file.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return 0 if successful, otherwise an error code.
 */
DEFINE_FUNC_1(hx_ffmpeg_av_find_best_video_stream, context)
{
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_av_find_video_stream(contextPointer);

  return alloc_int(result);
}

/**
 * @brief Locates and caches the best audio stream in the file.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return 0 if successful, otherwise an error code.
 */
DEFINE_FUNC_1(hx_ffmpeg_av_find_best_audio_stream, context)
{
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_av_find_audio_stream(contextPointer);

  return alloc_int(result);
}

/**
 * @brief Locates and caches the best subtitle stream in the file.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return 0 if successful, otherwise an error code.
 */
DEFINE_FUNC_1(hx_ffmpeg_av_find_best_subtitle_stream, context)
{
  // printf("[extension-ffmpeg] av_find_best_subtitle_stream\n");
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_av_find_subtitle_stream(contextPointer);

  return alloc_int(result);
}

/**
 * @brief Returns the width of the current video stream.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return The width of the video stream in pixels.
 */
DEFINE_FUNC_1(hx_ffmpeg_get_video_width, context)
{
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  
  int result = contextPointer->videoCodecCtx->width;
  return alloc_int(result);
}

/**
 * @brief Returns the height of the current video stream.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return The height of the video stream in pixels.
 */
DEFINE_FUNC_1(hx_ffmpeg_get_video_height, context)
{
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  
  int result = contextPointer->videoCodecCtx->height;
  return alloc_int(result);
}