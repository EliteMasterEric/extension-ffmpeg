#include "Format.h"

/**
 * @brief Opens an input file and stores the format context.
 * 
 * @param context The FFmpegContext to populate.
 * @param filename The name of the file to open.
 * @return Whether the file was opened successfully.
 */
int FFmpegContext_avformat_open_input(FFmpegContext* context, const char* filename) {
  context->avFormatCtx = avformat_alloc_context();

  int result = avformat_open_input(&context->avFormatCtx, filename, NULL, NULL);

  return result;
}

/**
 * @brief Opens an input file, allowing avformat to determine file format information.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @param filename A Haxe object wrapping the name of the file to open.
 * @return Whether the file was opened successfully.
 */
value __hx_ffmpeg_avformat_open_input(value context, value filename) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  const char* filenamePointer = val_get_string(filename);
  int result = FFmpegContext_avformat_open_input(contextPointer, filenamePointer);

  return alloc_int(result);
}

DEFINE_FUNC_2(hx_ffmpeg_avformat_open_input, context, filename)
{
  return __hx_ffmpeg_avformat_open_input(context, filename);
}

/**
 * @brief Parses additional information about the stream,
 *        such as the frame rate.
 * 
 * @param context The FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
int FFmpegContext_avformat_find_stream_info(FFmpegContext* context) {
  int result = avformat_find_stream_info(context->avFormatCtx, NULL);

  return result;
}

/**
 * @brief Parses additional information about the stream,
 *        such as the frame rate.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
value __hx_ffmpeg_avformat_find_stream_info(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_avformat_find_stream_info(contextPointer);

  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_avformat_find_stream_info, context)
{
  return __hx_ffmpeg_avformat_find_stream_info(context);
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
    printf("[extension-ffmpeg] Found video stream %d.\n", videoStreamIndex);
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
    printf("[extension-ffmpeg] Found audio stream %d.\n", audioStreamIndex);
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
    printf("[extension-ffmpeg] Found subtitle stream %d.\n", subtitleStreamIndex);
    context->subtitleStreamIndex = subtitleStreamIndex;
    context->subtitleStream = context->avFormatCtx->streams[subtitleStreamIndex];
    context->subtitleCodec = subtitleCodec;
  } else {
    // An error occurred.
    printf("[extension-ffmpeg] No subtitle stream\n");
  }
  return subtitleStreamIndex;
}

/**
 * @brief Parses additional information about the stream,
 *        such as the frame rate.
 * 
 * @param context The FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
void FFmpegContext_av_dump_format(FFmpegContext* context) {
  av_dump_format(context->avFormatCtx, 0, "debug.log", 0);
}

/**
 * @brief Parses additional information about the stream,
 *        such as the frame rate.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
value __hx_ffmpeg_av_dump_format(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  FFmpegContext_av_dump_format(contextPointer);

  return alloc_null();
}

DEFINE_FUNC_1(hx_ffmpeg_av_dump_format, context)
{
  return __hx_ffmpeg_av_dump_format(context);
}

/**
 * @brief Locates and caches the best video stream in the file.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
value __hx_ffmpeg_av_find_best_video_stream(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_av_find_video_stream(contextPointer);

  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_av_find_best_video_stream, context)
{
  return __hx_ffmpeg_av_find_best_video_stream(context);
}

/**
 * @brief Locates and caches the best audio stream in the file.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
value __hx_ffmpeg_av_find_best_audio_stream(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_av_find_audio_stream(contextPointer);

  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_av_find_best_audio_stream, context)
{
  return __hx_ffmpeg_av_find_best_audio_stream(context);
}

/**
 * @brief Locates and caches the best subtitle stream in the file.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the file was parsed successfully.
 */
value __hx_ffmpeg_av_find_best_subtitle_stream(value context) {
  printf("[extension-ffmpeg] av_find_best_subtitle_stream\n");
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  int result = FFmpegContext_av_find_subtitle_stream(contextPointer);

  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_av_find_best_subtitle_stream, context)
{
  return __hx_ffmpeg_av_find_best_subtitle_stream(context);
}

/**
 * @brief Returns the width of the current video stream.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return The width of the video stream in pixels.
 */
value __hx_ffmpeg_get_video_width(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  
  int result = contextPointer->videoCodecCtx->width;
  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_get_video_width, context)
{
  return __hx_ffmpeg_get_video_width(context);
}

/**
 * @brief Returns the height of the current video stream.
 * 
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return The height of the video stream in pixels.
 */
value __hx_ffmpeg_get_video_height(value context) {
  FFmpegContext* contextPointer = FFmpegContext_unwrap(context);
  
  int result = contextPointer->videoCodecCtx->height;
  return alloc_int(result);
}

DEFINE_FUNC_1(hx_ffmpeg_get_video_height, context)
{
  return __hx_ffmpeg_get_video_height(context);
}