#define FFMPEG_CORE_CPP
#include "Core.h"

extern "C" void initialize()
{
  initialize_Structures();
}

/**
 * @brief Throw a string as a Haxe exception.
 */
void hx_throw_exception(const char *message)
{
  val_throw(alloc_string(message));
}