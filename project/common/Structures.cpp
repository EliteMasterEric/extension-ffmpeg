#include "Structures.h"

/**
 * @brief Initialize structures, typedefs, and kinds.
 */
void initialize_Structures() {
  kind_FFmpegContext = alloc_kind();
}

bool is_FFmpegContext(value input) {
  return val_is_kind(input, kind_FFmpegContext);
}

/**
 * @brief Create a new FFmpegContext object. This stores all the information
 * about a video or audio stream.
 * 
 * @return A pointer to a new FFmpegContext struct.
 */
FFmpegContext* FFmpegContext_create() {
  FFmpegContext* contextPointer = (FFmpegContext*) malloc(sizeof(FFmpegContext));
  return contextPointer;
}

/**
 * @brief Unwraps a Haxe object into an FFmpegContext object.
 */
FFmpegContext* FFmpegContext_unwrap(value input) {
  if (!is_FFmpegContext(input)) hx_throw_exception("Not an FFmpegContext.");
  return (FFmpegContext*) val_get_handle(input, kind_FFmpegContext);
}

/**
 * @brief Process an FFmpegContext for garbage collection.
 * This destroys an object that is no longer needed.
 */
static void FFmpegContext_destroy(value input) {
   FFmpegContext* contextPointer = FFmpegContext_unwrap(input);
}

/**
 * @brief Wraps an FFmpegContext object in a Haxe object.
 * @return A Haxe object representing the FFmpegContext.
 */
value FFmpegContext_wrap(FFmpegContext* input) {
  // Wrap the FFmpegContext object in a Haxe Dynamic so it can be held onto.
  value result = alloc_abstract(kind_FFmpegContext, input);

  // Ensure the object is garbage collected when the Haxe object is destroyed.
  val_gc(result, FFmpegContext_destroy);
  
  return result;
}

/**
 * @brief Instantiates an (empty) FFmpegContext object.
 * 
 * @return The FFmpegContext object.
 */
const value __hx_ffmpeg_init_ffmpegcontext()
{
  return FFmpegContext_wrap(FFmpegContext_create());
}
DEFINE_FUNC_0(hx_ffmpeg_init_ffmpegcontext)
{
  return __hx_ffmpeg_init_ffmpegcontext();
}
