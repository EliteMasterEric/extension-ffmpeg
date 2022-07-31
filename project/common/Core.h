/**
 * @file Core.h
 * @author MasterEric
 * @brief Defines shared types and functions used in the rest of the project. 
 */

#ifndef EXT_FFMPEG_CORE
#define EXT_FFMPEG_CORE

// Standard CPP includes
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstdlib>
#include <assert.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

// Library includes
extern "C"
{
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>

  #include <libavcodec/version.h>
  #include <libavformat/version.h>
  #include <libavutil/version.h>
  #include <libavfilter/version.h>
  #include <libswresample/version.h>
  #include <libswscale/version.h>
}

// Only include IMPLEMENT_API in ONE .cpp file.
// Otherwise you get a linker error.
#if !defined(STATIC_LINK) && defined(FFMPEG_CORE_CPP)
#define IMPLEMENT_API
#endif

// Intergration with Haxe
#include <hx/CFFI.h>

// Standard macros and type definitions.
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#ifndef NULL
#define NULL 0
#endif

#ifndef HX_WINDOWS
#include <stdint.h>
#else
typedef uint64 uint64_t;
#endif

/**
 * Macros for definition of external interface functions, callable by Haxe.
 * To define a function with 0 arguments, use `DEFFUNC_0(return, function_name)`
 * To define a function with 3 arguments, use `DEFFUNC_3(return, function_name, param_1, param_2, param_3)`
 */
#define DEFINE_FUNC(COUNT, NAME, ...) \
  value NAME(__VA_ARGS__);            \
  DEFINE_PRIM(NAME, COUNT);           \
  value NAME(__VA_ARGS__)
#define DEFINE_FUNC_0(NAME) DEFINE_FUNC(0, NAME)
#define DEFINE_FUNC_1(NAME, PARAM1) DEFINE_FUNC(1, NAME, value PARAM1)
#define DEFINE_FUNC_2(NAME, PARAM1, PARAM2) DEFINE_FUNC(2, NAME, value PARAM1, value PARAM2)
#define DEFINE_FUNC_3(NAME, PARAM1, PARAM2, PARAM3) DEFINE_FUNC(3, NAME, value PARAM1, value PARAM2, value PARAM3)
#define DEFINE_FUNC_4(NAME, PARAM1, PARAM2, PARAM3, PARAM4) DEFINE_FUNC(4, NAME, value PARAM1, value PARAM2, value PARAM3, value PARAM4)
#define DEFINE_FUNC_5(NAME, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5) DEFINE_FUNC(5, NAME, value PARAM1, value PARAM2, value PARAM3, value PARAM4, value PARAM5)

/**
 * An FFmpegContext struct represents the state of a given video or audio stream.
 * A new one is created when a file is loaded, and accumulates and caches
 * information about the stream as it becomes needed.
 */
typedef struct {
	AVFormatContext* avFormatCtx;
} FFmpegContext;

/**
 * @brief Throws an exception in Haxe.
 * 
 * @param message The message to send.
 */
void hx_throw_exception(const char* message);

/**
 * @brief Called when the library is instantiated.
 */
extern "C" void initialize();

/**
 * @brief Initialize structures, typedefs, and kinds.
 */
void initialize_Structures();

// Define functions here to share them throughout the project.
bool is_FFmpegContext(value v);
FFmpegContext* FFmpegContext_unwrap(value input);

// Make sure the value below are defined only once.
#if defined(FFMPEG_CORE_CPP)
DEFINE_ENTRY_POINT(initialize);
#endif

#endif // EXT_FFMPEG_CORE