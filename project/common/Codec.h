/**
 * @file Codec.h
 * @author MasterEric
 * @brief Defines CFFI functions for initializing and working with codecs.
 */

#ifndef EXT_FFMPEG_CODEC
#define EXT_FFMPEG_CODEC

#include "Core.h"

int FFmpegContext_allocate_video_frame(FFmpegContext *context);

#endif // EXT_FFMPEG_CODEC