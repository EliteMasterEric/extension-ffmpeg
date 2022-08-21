#include "Structures.h"

/**
 * @brief Initialize structures, typedefs, and kinds.
 *        Called when the library is loaded.
 */
void initialize_Structures()
{
  kind_FFmpegContext = alloc_kind();
}

/**
 * @brief Determine whether the provided `value` is a `FFmpegContext`.
 *
 * @param value The `value` to check.
 */
bool is_FFmpegContext(value input)
{
  return val_is_kind(input, kind_FFmpegContext);
}

/**
 * @brief Create a new FFmpegContext object.
 *        This stores all the information about a video or audio stream.
 *
 * @return A pointer to a new FFmpegContext struct.
 */
FFmpegContext *FFmpegContext_create(int videoQueueSize, int audioQueueSize)
{
  // printf("Allocating FFmpegContext...\n");
  FFmpegContext *contextPointer = (FFmpegContext *)malloc(sizeof(FFmpegContext));

  // We must ensure all values are initialized and allocated to prevent runtime errors.

  contextPointer->avFormatCtx = avformat_alloc_context();

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

  contextPointer->videoOutputFrame = av_frame_alloc();
  contextPointer->videoOutputFrameBuffer = nullptr;

  // printf("Creating video and audio frame queues...\n");
  contextPointer->videoFrameQueue = FFmpegFrameQueue_create(videoQueueSize);
  // printf("Created video queue\n");
  contextPointer->audioFrameQueue = FFmpegFrameQueue_create(audioQueueSize);
  // printf("Created audio queue\n");

  contextPointer->audioOutputChannelLayout = FFMPEG_CHANNEL_LAYOUT;

  contextPointer->audioOutputFrameBuffer = nullptr;
  contextPointer->audioOutputFrameSize = 0;
  contextPointer->audioOutputLeftoverBuffer = nullptr;
  contextPointer->audioOutputLeftoverBufferSize = 0;

  contextPointer->swsCtx = nullptr;
  contextPointer->swrCtx = nullptr;

  contextPointer->decodeThread = NULL;
  contextPointer->audioThread = NULL;
  contextPointer->videoThread = NULL;

  contextPointer->quit = false;

  // printf("Completed allocating FFmpegContext\n");

  return contextPointer;
}

DEFINE_FUNC_2(hx_ffmpeg_init_ffmpegcontext, videoQueueValue, audioQueueValue)
{
  // Use the provided values.
  int videoQueueSize = val_int(videoQueueValue);
  int audioQueueSize = val_int(audioQueueValue);
  return FFmpegContext_wrap(FFmpegContext_create(videoQueueSize, audioQueueSize));
}

/**
 * @brief Unwraps a Haxe object into an FFmpegContext object.
 *
 * @param input The Haxe object to unwrap.
 * @return The FFmpegContext object.
 */
FFmpegContext *FFmpegContext_unwrap(value input)
{
  if (!is_FFmpegContext(input))
    hx_throw_exception("Not an FFmpegContext.");
  return (FFmpegContext *)val_get_handle(input, kind_FFmpegContext);
}

void hx_ffmpegcontext_close(value context)
{
  FFmpegContext_close(FFmpegContext_unwrap(context));
}

/**
 * @brief Wraps an FFmpegContext object in a Haxe object.
 *
 * @param context The FFmpegContext object to wrap.
 * @return A Haxe object containing a pointer to the FFmpegContext.
 */
value FFmpegContext_wrap(FFmpegContext *input)
{
  // Wrap the FFmpegContext object in a Haxe Dynamic so it can be held onto.
  value result = alloc_abstract(kind_FFmpegContext, input);

  // Ensure the object is garbage collected when the Haxe object is destroyed.
  val_gc(result, hx_ffmpegcontext_close);

  return result;
}

/**
 * @brief Cleans up an FFmpegContext object.
 *        Closes the file and codec, and frees the memory.
 *        Also signals any threads to close.
 */
void FFmpegContext_close(FFmpegContext *context)
{
  // Free the frames.
  av_free(context->videoOutputFrameBuffer);
  av_free(context->videoOutputFrame);

  // Close the video codec.
  avcodec_close(context->videoCodecCtx);

  // Close the file.
  avformat_close_input(&context->avFormatCtx);

  context->quit = true;
}

DEFINE_FUNC_1(hx_ffmpeg_close_ffmpegcontext, context)
{
  FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
  FFmpegContext_close(contextPointer);
  return alloc_null();
}
