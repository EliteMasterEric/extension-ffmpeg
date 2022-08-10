/**
 * @file Core.h
 * @author MasterEric
 * @brief Imports libraries, and defines shared types and functions used in the rest of the project.
 */

#ifndef EXT_FFMPEG_CORE
#define EXT_FFMPEG_CORE

// Standard CPP includes
#include <assert.h>
#include <cstdlib>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <condition_variable>
#include <mutex>
#include <thread>

// Library includes
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/version.h>

#include <libavdevice/avdevice.h>
#include <libavdevice/version.h>

#include <libavfilter/avfilter.h>
#include <libavfilter/version.h>

#include <libavformat/avformat.h>
#include <libavformat/version.h>

#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>

#include <libswresample/swresample.h>
#include <libswresample/version.h>

#include <libswscale/swscale.h>
#include <libswscale/version.h>
}

#define FFMPEG_PIXEL_FORMAT AV_PIX_FMT_BGRA
#define FFMPEG_BITRATE 44100
#define FFMPEG_CHANNEL_LAYOUT AV_CHANNEL_LAYOUT_STEREO
#define FFMPEG_VIDEO_QUEUE_SIZE 8
#define FFMPEG_AUDIO_QUEUE_SIZE 16

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

// Prime notes:
//
// int: "i"
// float: "f"
// char*: "c"
// value (Dynamic): "o"
// Haxe string: "s"
// void: "v"
//
// DEFINE_PRIME1(NAME) for a function with 1 argument
// DEFINE_PRIME1v(NAME) for a function with 1 argument and a void return type

typedef struct FFmpegFrameQueue
{
  // The queue, containing an array of pointers to raw (decoded) frames.
  AVFrame **queue;

  // The current number of frames in the queue.
  int size;
  // The maximum number of frames that can be stored in the queue.
  int maxSize;
  // The next index in the queue to write to.
  int readIndex;
  // The next index in the queue to read from.
  int writeIndex;

  // Syncronization mutex.
  std::mutex mutex;
  std::condition_variable cond;

  // Assignment operator
  FFmpegFrameQueue &operator=(const FFmpegFrameQueue &o)
  {
    queue = o.queue;

    size = o.size;
    maxSize = o.maxSize;
    readIndex = o.readIndex;
    writeIndex = o.writeIndex;

    // SKIP mutex and cond
    // They are unassignable.

    return *this;
  }

  // Constructor
  FFmpegFrameQueue()
  {
    queue = NULL;

    size = 0;
    maxSize = 0;
    readIndex = 0;
    writeIndex = 0;

    // SKIP mutex and cond
    // They are unassignable.
  }

  // Destructor
  ~FFmpegFrameQueue() {}
} FFmpegFrameQueue;

/**
 * An FFmpegContext struct represents the state of a given media stream.
 * A new one is created when a file is loaded, and accumulates and caches
 * information about the stream as it becomes needed.
 */
typedef struct
{
  // The format context.
  // This acts as an abstraction for the CONTAINER of the input file.
  AVFormatContext *avFormatCtx = nullptr;

  // Information about the video stream.
  int videoStreamIndex;
  AVStream *videoStream;
  const AVCodec *videoCodec;
  AVCodecContext *videoCodecCtx;

  // Information about the audio stream.
  int audioStreamIndex;
  AVStream *audioStream;
  const AVCodec *audioCodec;
  AVCodecContext *audioCodecCtx;

  // Information about the subtitle stream.
  int subtitleStreamIndex;
  AVStream *subtitleStream;
  const AVCodec *subtitleCodec;
  AVCodecContext *subtitleCodecCtx;

  // The frame queue for the video stream.
  FFmpegFrameQueue* videoFrameQueue;
  AVFrame *videoOutputFrame;
  size_t videoOutputFrameSize;
  uint8_t *videoOutputFrameBuffer;

  // The frame queue for the audio stream.
  FFmpegFrameQueue* audioFrameQueue;
  buffer audioOutputBuffer;
  AVChannelLayout audioOutputChannelLayout;

  // The software scaler context.
  struct SwsContext *swsCtx;

  // The software resampler context.
  struct SwrContext *swrCtx;

  // The parallel processing threads.
  std::thread* decodeThread;
  std::thread* audioThread;
  std::thread* videoThread;

  // Set to true when the FFmpegContext is ready to be destroyed.
  bool quit;
} FFmpegContext;

/**
 * @brief Called when the library is instantiated.
 */
extern "C" void initialize();

/**
 * @brief Initialize structures, typedefs, and kinds.
 */
void initialize_Structures();

//
// Define functions here to share them throughout the project.
//

AVFrame *FFmpegFrameQueue_pop(FFmpegFrameQueue* queue);
bool is_FFmpegContext(value v);
FFmpegContext *FFmpegContext_unwrap(value input);
int FFmpeg_emit_audio_frame(FFmpegContext *context, value callback);
int FFmpegContext_init_swrCtx(FFmpegContext *context);
int FFmpegContext_init_swsCtx(FFmpegContext *context);
int FFmpegContext_swr_resample_audio_frame(FFmpegContext *context);
int FFmpegContext_sws_scale_video_frame(FFmpegContext *context);
int FFmpegFrameQueue_size(FFmpegFrameQueue* queue);
void FFmpegFrameQueue_create(FFmpegFrameQueue* queue, int maxSize);
void FFmpegFrameQueue_push(FFmpegFrameQueue* queue, AVFrame *frame);
void hx_throw_exception(const char *message);
buffer cffi_build_buffer(unsigned char* data, int length);

#if defined(FFMPEG_CORE_CPP)
//
// Define functions here to use them ONLY in Core.cpp.
//
DEFINE_ENTRY_POINT(initialize);
#endif

#endif // EXT_FFMPEG_CORE