#include "Version.h"

/**
 * @brief Get the version of the `avcodec` library.
 */
const char *__hx_ffmpeg_get_libavcodec_version()
{
  return AV_STRINGIFY(LIBAVCODEC_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavcodec_version)
{
  return alloc_string(__hx_ffmpeg_get_libavcodec_version());
}

/**
 * @brief Get the version of the `avformat` library.
 */
const char *__hx_ffmpeg_get_libavformat_version()
{
  return AV_STRINGIFY(LIBAVFORMAT_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavformat_version)
{
  return alloc_string(__hx_ffmpeg_get_libavformat_version());
}

/**
 * @brief Get the version of the `avutil` library.
 */
const char *__hx_ffmpeg_get_libavutil_version()
{
  return AV_STRINGIFY(LIBAVUTIL_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavutil_version)
{
  return alloc_string(__hx_ffmpeg_get_libavutil_version());
}

/**
 * @brief Get the version of the `avfilter` library.
 */
const char *__hx_ffmpeg_get_libavfilter_version()
{
  return AV_STRINGIFY(LIBAVFILTER_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavfilter_version)
{
  return alloc_string(__hx_ffmpeg_get_libavfilter_version());
}

/**
 * @brief Get the version of the `swresample` library.
 */
const char *__hx_ffmpeg_get_libswresample_version()
{
  return AV_STRINGIFY(LIBSWRESAMPLE_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libswresample_version)
{
  return alloc_string(__hx_ffmpeg_get_libswresample_version());
}

/**
 * @brief Get the version of the `swscale` library.
 */
const char *__hx_ffmpeg_get_libswscale_version()
{
  return AV_STRINGIFY(LIBSWSCALE_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libswscale_version)
{
  return alloc_string(__hx_ffmpeg_get_libswscale_version());
}

/**
 * @brief Get the build flags used when building `avcodec`.
 */
const char *__hx_ffmpeg_get_avcodec_configuration()
{
  return avcodec_configuration();
}

DEFINE_FUNC_0(hx_ffmpeg_get_avcodec_configuration)
{
  return alloc_string(__hx_ffmpeg_get_avcodec_configuration());
}

/**
 * @brief Get the license of the `avcodec` library.
 */
const char *__hx_ffmpeg_get_avcodec_license()
{
  return avcodec_license();
}

DEFINE_FUNC_0(hx_ffmpeg_get_avcodec_license)
{
  return alloc_string(__hx_ffmpeg_get_avcodec_license());
}