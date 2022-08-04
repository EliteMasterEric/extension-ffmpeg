package ffmpeg;

import ffmpeg._internal.util.CppUtil;

enum FFmpegError {
    /**
     * FFmpeg was provided with an invalid URL or file path, and could not open the file.
     */
    EMediaNotFound(msg:String);
    /**
     * You attempted to access a file by URL, but the URL protocol is unknown,
     * or unsupported.
     */
    EProtocolNotFound(msg:String);
    /**
     * An unhandled error has occurred. Look up the error code for more information.
     * If you encounter this, please report it to the `extension-ffmpeg` repository,
     * so that more verbose messages can be provided.
     */
    EUnknown(code:Int, msg:String);
}

class Error {
    static final FFMPEG_MEDIA_NOT_FOUND = -2;
    static final AVERROR_PROTOCOL_NOT_FOUND = buildErrorCode(null, 'P', 'R', 'O');

    public static inline function handleError(code:Int):Void {
        if (code < 0) {
            throw buildError(code);
        }
    }

    static function buildError(code:Int):FFmpegError {
        var msg = interpretError(code);
        switch(code) {
            case FFMPEG_MEDIA_NOT_FOUND:
                return EMediaNotFound(msg);
            case AVERROR_PROTOCOL_NOT_FOUND:
                return EProtocolNotFound(msg);
            default:
                return EUnknown(code, msg);
        }
    }

    static function interpretError(code:Int):String {
        return CppUtil.loadFunction("hx_ffmpeg_av_strerror", 1)(code);
    }

    static inline function buildErrorCode(str1:Null<String>, str2:String, str3:String, str4:String):Int {
        return -1 * (((str1 == null ? 0xF8 : str1.charCodeAt(0)) << 0)
            + (str2.charCodeAt(0) << 8)
            + (str3.charCodeAt(0) << 16)
            + (str4.charCodeAt(0) << 24));
    }
}