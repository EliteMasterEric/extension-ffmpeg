#include "Version.h"

const char *__hx_ffmpeg_get_libavcodec_version()
{
  return AV_STRINGIFY(LIBAVCODEC_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavcodec_version)
{
  return alloc_string(__hx_ffmpeg_get_libavcodec_version());
}

const char *__hx_ffmpeg_get_libavformat_version()
{
  return AV_STRINGIFY(LIBAVFORMAT_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavformat_version)
{
  return alloc_string(__hx_ffmpeg_get_libavformat_version());
}

const char *__hx_ffmpeg_get_libavutil_version()
{
  return AV_STRINGIFY(LIBAVUTIL_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavutil_version)
{
  return alloc_string(__hx_ffmpeg_get_libavutil_version());
}

const char *__hx_ffmpeg_get_libavfilter_version()
{
  return AV_STRINGIFY(LIBAVFILTER_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libavfilter_version)
{
  return alloc_string(__hx_ffmpeg_get_libavfilter_version());
}

const char *__hx_ffmpeg_get_libswresample_version()
{
  return AV_STRINGIFY(LIBSWRESAMPLE_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libswresample_version)
{
  return alloc_string(__hx_ffmpeg_get_libswresample_version());
}

const char *__hx_ffmpeg_get_libswscale_version()
{
  return AV_STRINGIFY(LIBSWSCALE_VERSION);
}

DEFINE_FUNC_0(hx_ffmpeg_get_libswscale_version)
{
  return alloc_string(__hx_ffmpeg_get_libswscale_version());
}

const char *__hx_ffmpeg_get_avcodec_configuration()
{
  return avcodec_configuration();
}

DEFINE_FUNC_0(hx_ffmpeg_get_avcodec_configuration)
{
  return alloc_string(__hx_ffmpeg_get_avcodec_configuration());
}

const char *__hx_ffmpeg_get_avcodec_license()
{
  return avcodec_license();
}

DEFINE_FUNC_0(hx_ffmpeg_get_avcodec_license)
{
  return alloc_string(__hx_ffmpeg_get_avcodec_license());
}