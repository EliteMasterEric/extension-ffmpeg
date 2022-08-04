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