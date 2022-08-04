package ffmpeg;

import ffmpeg._internal.util.CppUtil;

/**
 * Represents a media file accessible by FFmpeg.
 */
class Media {
    private var context:Dynamic;

    public function new() {
        context = buildContext();
    }

    /**
     * Build an FFmpegContext object, used to hold information about the media.
     * @throws FFmpegError if the context could not be initialized.
     */
    private function buildContext():Dynamic {
        return CppUtil.loadFunction("hx_ffmpeg_init_ffmpegcontext", 0)();
    }

    /**
     * Open a media file via URL. Any protocol may be used (including `file:` and `https:`).
     * @throws FFmpegError if the media file could not be opened.
     */
    public function openInput(url:String):Void {
        var result:Int = CppUtil.loadFunction("hx_ffmpeg_avformat_open_input", 2)(context, url);
        
        Error.handleError(result);
    }

    /**
     * Determine detailed information about a stream, such as the framerate.
     * @throws FFmpegError if the media file could not be queried.
     */
    public function fetchStreamInfo():Void {
        var result:Int = CppUtil.loadFunction("hx_ffmpeg_avformat_find_stream_info", 1)(context);

        Error.handleError(result);
    }

    /**
     * A debug utility function to dump detailed stream info to stdout.
     */
    public function dumpFormat():Void {
        CppUtil.loadFunction("hx_ffmpeg_av_dump_format", 1)(context);
    }
}