#include "Structures.h"

/**
 * @brief Initialize structures, typedefs, and kinds.
 */
void initialize_Structures()
{
  kind_FFmpegContext = alloc_kind();
}

/**
 * @brief Determine whether the provided `value` is a `FFmpegContext`.
 */
bool is_FFmpegContext(value input)
{
  return val_is_kind(input, kind_FFmpegContext);
}

/**
 * @brief Unwraps a Haxe object into an FFmpegContext object.
 */
FFmpegContext *FFmpegContext_unwrap(value input)
{
  if (!is_FFmpegContext(input))
    hx_throw_exception("Not an FFmpegContext.");
  return (FFmpegContext *)val_get_handle(input, kind_FFmpegContext);
}

/**
 * @brief Process an FFmpegContext for garbage collection.
 * This destroys an object that is no longer needed.
 */
static void FFmpegContext_destroy(value input)
{
  FFmpegContext *contextPointer = FFmpegContext_unwrap(input);
}

/**
 * @brief Wraps an FFmpegContext object in a Haxe object.
 * @return A Haxe object representing the FFmpegContext.
 */
value FFmpegContext_wrap(FFmpegContext *input)
{
  // Wrap the FFmpegContext object in a Haxe Dynamic so it can be held onto.
  value result = alloc_abstract(kind_FFmpegContext, input);

  // Ensure the object is garbage collected when the Haxe object is destroyed.
  val_gc(result, FFmpegContext_destroy);

  return result;
}

/**
 * @brief Create a new FFmpegContext object. This stores all the information
 * about a video or audio stream.
 *
 * @return A pointer to a new FFmpegContext struct.
 */
FFmpegContext *FFmpegContext_create()
{
  FFmpegContext *contextPointer = (FFmpegContext *)malloc(sizeof(FFmpegContext));

  // We must ensure all values are initialized to prevent comparision errors.

  contextPointer->videoStreamIndex = -1;
  contextPointer->videoStream = nullptr;
  contextPointer->videoCodec = nullptr;
  contextPointer->videoCodecCtx = nullptr;

  contextPointer->audioStreamIndex = -1;
  contextPointer->audioStream = nullptr;
  contextPointer->audioCodec = nullptr;
  contextPointer->audioCodecCtx = nullptr;

  contextPointer->subtitleStreamIndex = -1;
  contextPointer->subtitleStream = nullptr;
  contextPointer->subtitleCodec = nullptr;
  contextPointer->subtitleCodecCtx = nullptr;

  contextPointer->videoFrame = av_frame_alloc();
  contextPointer->videoFrameRGB = av_frame_alloc();
  contextPointer->videoFrameRGBBuffer = nullptr;
 
  contextPointer->audioFrame = av_frame_alloc();
  contextPointer->audioOutputBuffer = NULL;
  contextPointer->audioOutputChannelLayout = AV_CHANNEL_LAYOUT_STEREO;
  contextPointer->emitAudioCallback = NULL;

  contextPointer->swsCtx = nullptr;
  contextPointer->swrCtx = nullptr;

  return contextPointer;
}
const value __hx_ffmpeg_init_ffmpegcontext()
{
  return FFmpegContext_wrap(FFmpegContext_create());
}
DEFINE_FUNC_0(hx_ffmpeg_init_ffmpegcontext)
{
  return __hx_ffmpeg_init_ffmpegcontext();
}

/**
 * @brief Cleans up an FFmpegContext object. Closes the file and codec,
 * and frees the memory.
 */
void FFmpegContext_close(FFmpegContext *context)
{
  // Free the frames.
  av_free(context->videoFrameRGBBuffer);
  av_free(context->videoFrameRGB);
  av_free(context->videoFrame);
  av_free(context->audioFrame);

  // Close the video codec.
  avcodec_close(context->videoCodecCtx);

  // Close the file.
  avformat_close_input(&context->avFormatCtx);
}
const value __hx_ffmpeg_close_ffmpegcontext(value context)
{
  FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
  FFmpegContext_close(contextPointer);
  return alloc_null();
}
DEFINE_FUNC_1(hx_ffmpeg_close_ffmpegcontext, context)
{
  return __hx_ffmpeg_close_ffmpegcontext(context);
}
