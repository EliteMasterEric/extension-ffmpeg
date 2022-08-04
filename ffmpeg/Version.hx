package ffmpeg;

import cpp.Lib;

import ffmpeg._internal.util.CppUtil;

class Version {
	public static function getAvcodecConfiguration():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_avcodec_configuration", 0)();
	}

	public static function getAvcodecLicense():String {
		return CppUtil.loadFunction("hx_ffmpeg_get_avcodec_license", 0)();
	}

	public static function getLibavcodecVersion():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_libavcodec_version", 0)();
	}

	public static function getLibavfilterVersion():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_libavfilter_version", 0)();
	}

	public static function getLibavformatVersion():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_libavformat_version", 0)();
	}

	public static function getLibavutilVersion():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_libavutil_version", 0)();
	}

	public static function getLibswresampleVersion():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_libswresample_version", 0)();
	}

	public static function getLibswscaleVersion():String {
    return CppUtil.loadFunction("hx_ffmpeg_get_libswscale_version", 0)();
	}
}
