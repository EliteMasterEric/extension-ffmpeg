/**
 * @file DecodeVideo.h
 * @author MasterEric
 * @brief Defines CFFI functions for outputting video frames from a stream.
 */

#ifndef EXT_FFMPEG_DECODEVIDEO
#define EXT_FFMPEG_DECODEVIDEO

#include "Core.h"

int copyVideoFrame(AVFrame* videoFrame, char* buffer_ptr, int buffer_size);

#endif // EXT_FFMPEG_DECODEVIDEO