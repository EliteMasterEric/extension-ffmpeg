package ffmpeg;

import ffmpeg._internal.util.CppUtil;

enum FFmpegError {
  /**
   * FFmpeg was provided with an invalid URL or file path, and could not open the file.
   */
  EMediaNotFound;

  /**
   * The operation failed but you should try again.
   * For example, if you passed the decoder some data and it failed, you should try passing more data to it.
   */
  ETryAgain;

  /**
   * FFmpeg attempted to locate a specific type of stream in a file, but did not find any.
   * For example, requesting a video stream from a file that contains only audio.
   */
  EStreamNotFound;

  /**
   * Invalid data was encountered when processing the input stream.
   */
  EInvalidData;

  /**
   * You attempted to access a file by URL, but the URL protocol is unknown,
   * or unsupported.
   */
  EProtocolNotFound;

  /**
   * FFmpeg could not find a suitable decoder for the provided data.
   */
  EDecoderNotFound;

  /**
   * FFmpeg could not find a suitable encoder for the provided data.
   */
  EEncoderNotFound;

  /**
   * FFmpeg could not find a suitable demuxer for the provided data.
   */
  EDemuxerNotFound;

  /**
   * FFmpeg could not find a suitable muxer for the provided data.
   */
  EMuxerNotFound;

  /**
   * FFmpeg reached the end of the media file.
   * This is normal and will happen when the last frame of the file has been decoded.
   */
  EEndOfFile;

  /**
   * An unhandled error has occurred. Look up the error code for more information.
   * If you encounter this, please report it to the `extension-ffmpeg` repository,
   * so that more verbose messages can be provided.
   */
  EUnknown(code:Int, msg:String);
}

class Error {
  public static final FFMPEG_NO_PERMISSION = -1; // EPERM
  public static final FFMPEG_MEDIA_NOT_FOUND = -2; // ENOENT

  public static final FFMPEG_TRY_AGAIN = -11; // EAGAIN
  public static final FFMPEG_TRY_AGAIN2 = -35; // EAGAIN2

  public static final AVERROR_PROTOCOL_NOT_FOUND = buildErrorCode(null, 'P', 'R', 'O');
  public static final AVERROR_STREAM_NOT_FOUND = buildErrorCode(null, 'S', 'T', 'R');
  public static final AVERROR_END_OF_FILE = buildErrorCode('E', 'O', 'F', ' ');
  public static final AVERROR_INVALID_DATA = buildErrorCode('I', 'N', 'D', 'A');
  public static final AVERROR_DECODER_NOT_FOUND = buildErrorCode(null, 'D', 'E', 'C');
  public static final AVERROR_ENCODER_NOT_FOUND = buildErrorCode(null, 'E', 'N', 'C');
  public static final AVERROR_DEMUXER_NOT_FOUND = buildErrorCode(null, 'D', 'E', 'M');
  public static final AVERROR_MUXER_NOT_FOUND = buildErrorCode(null, 'M', 'U', 'X');

  /*
    ESRCH      3
    EINTR      4
    EIO        5
    ENXIO      6
    E2BIG      7
    ENOEXEC    8
    EBADF      9
    ECHILD    10
    ENOMEM    12
    EACCES    13
    EFAULT    14
    ENOTBLK   15
    EBUSY     16
    EEXIST    17
    EXDEV     18
    ENODEV    19
    ENOTDIR   20
    EISDIR    21
    EINVAL    22
    ENFILE    23
    EMFILE    24
    ENOTTY    25
    ETXTBSY   26
    EFBIG     27
    ENOSPC    28
    ESPIPE    29
    EROFS     30
    EMLINK    31
    EPIPE     32
    EDOM      33
    ERANGE    34
   */
   
  public static inline function handleError(code:Int):Void {
    if (code < 0) {
      throw buildError(code);
    }
  }

  static function buildError(code:Int):FFmpegError {
    switch (code) {
      case FFMPEG_MEDIA_NOT_FOUND:
        return EMediaNotFound;
      case FFMPEG_TRY_AGAIN:
        return ETryAgain;
      case FFMPEG_TRY_AGAIN2:
        return ETryAgain;
      case AVERROR_STREAM_NOT_FOUND:
        return EStreamNotFound;
      case AVERROR_END_OF_FILE:
        return EEndOfFile;
      case AVERROR_PROTOCOL_NOT_FOUND:
        return EProtocolNotFound;
      case AVERROR_INVALID_DATA:
        return EInvalidData;
      case AVERROR_DECODER_NOT_FOUND:
        return EDecoderNotFound;
      case AVERROR_ENCODER_NOT_FOUND:
        return EEncoderNotFound;
      case AVERROR_DEMUXER_NOT_FOUND:
        return EDemuxerNotFound;
      case AVERROR_MUXER_NOT_FOUND:
        return EMuxerNotFound;

      default:
        return EUnknown(code, interpretError(code));
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
