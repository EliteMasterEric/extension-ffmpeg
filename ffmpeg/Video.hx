package ffmpeg;

import ffmpeg._internal.util.CppUtil;

class Video {
  public static function buildContext():Dynamic {
    return CppUtil.loadFunction("hx_ffmpeg_init_ffmpegcontext", 0)();
  }

  public static function openInput(context:Dynamic, filename:String):Bool {
    return CppUtil.loadFunction("hx_ffmpeg_open_input", 2)(context, filename);
  }

  public static function streamInfo(context:Dynamic):Bool {
    return CppUtil.loadFunction("hx_ffmpeg_open_input", 1)(context);
  }
}