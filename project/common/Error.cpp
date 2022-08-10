#include "Error.h"

/**
 * @brief Fetches the error message for the given error code.
 *
 * @param errValue The Haxe object wrapping error code to fetch the message for.
 * @return A Haxe object wrapping error message string for the given error code.
 */
DEFINE_FUNC_1(hx_ffmpeg_av_strerror, errValue)
{
  int errCode = val_get_int(errValue);

  char errMsg[1024] = {0};
  av_strerror(errCode, errMsg, 1024);

  return alloc_string(errMsg);
}