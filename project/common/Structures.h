/**
 * @file Structures.h
 * @author MasterEric
 * @brief Defines CFFI functions to instantiate and cleanup structured objects.
 */

#ifndef EXT_FFMPEG_STRUCTURES
#define EXT_FFMPEG_STRUCTURES

#include "Core.h"

/**
 * `vkind` objects are used by alloc_abstract to allow for type comparison
 * on `value` objects which were previously stored.
 */
vkind kind_FFmpegContext;

value FFmpegContext_wrap(FFmpegContext *input);
void FFmpegContext_close(FFmpegContext *context);

#endif // EXT_FFMPEG_STRUCTURES