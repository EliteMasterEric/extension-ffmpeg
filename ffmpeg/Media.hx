package ffmpeg;

import ffmpeg._internal.util.CppUtil;
import ffmpeg.Error;
import ffmpeg.Error.FFmpegError;
import openfl.display.BitmapData;
import haxe.io.BytesData;

/**
 * Represents a media file accessible by FFmpeg.
 */
class Media {
  private var context:Dynamic;

  /**
   * Whether the media has a valid video stream.
   */
  public var hasVideo(default, null):Bool = false;

  /**
   * Whether the media has a valid audio stream.
   */
  public var hasAudio(default, null):Bool = false;

  /**
   * Whether the media has a valid subtitle stream.
   */
  public var hasSubtitle(default, null):Bool = false;

  /**
   * The width of the video associated with this media file.
   * Value is `-1` (invalid) if the media has no stream or the codec is not initialized yet.
   */
  public var videoWidth(default, null):Int = -1;

  /**
   * The width of the video associated with this media file.
   * Value is `-1` (invalid) if the media has no stream or the codec is not initialized yet.
   */
  public var videoHeight(default, null):Int = -1;

  /**
   * The bitmap data for the current video frame.
   */
  public var videoBitmapData(default, null):BitmapData;

  /**
   * A callback function that is called when the video codec is initialized,
   * and all video properties are available.
   */
  public var onInitVideoCodec:Void->Void = null;

  /**
   * A callback function that is called when the `videoBitmapData` has been filled
   * with the current video frame.
   */
  public var onEmitVideoFrame:Void->Void = null;

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

    hasVideo = fetchVideoStream();
    hasAudio = fetchAudioStream();
    hasSubtitle = fetchSubtitleStream();

    trace('fetched streams');
  }

  /**
   * Search the media file for the best video stream.
   */
  function fetchVideoStream():Bool {
    try {
      var resultB:Int = CppUtil.loadFunction("hx_ffmpeg_av_find_best_video_stream", 1)(context);
      Error.handleError(resultB);
      return true;
    }
    catch (e:FFmpegError) {
      // An issue with locating the video stream.
      switch (e) {
        case EStreamNotFound:
          // This is expected if the input file is not a video file.
          return false;
        default:
          throw e;
      }
    }
  }

  /**
   * Search the media file for the best audio stream.
   */
  function fetchAudioStream():Bool {
    try {
      var resultC:Int = CppUtil.loadFunction("hx_ffmpeg_av_find_best_audio_stream", 1)(context);
      Error.handleError(resultC);
      return true;
    }
    catch (e:FFmpegError) {
      // An issue with locating the audio stream.
      switch (e) {
        case EStreamNotFound:
          // This is expected if the input file is not an audio file.
          return false;
        default:
          throw e;
      }
    }
  }

  /**
   * Search the media file for the best subtitle stream.
   */
  function fetchSubtitleStream() {
    try {
      var result:Int = CppUtil.loadFunction("hx_ffmpeg_av_find_best_subtitle_stream", 1)(context);
      Error.handleError(result);
      return true;
    }
    catch (e:FFmpegError) {
      // An issue with locating the subtitle stream.
      switch (e) {
        case EStreamNotFound:
          // This is expected if the input file has no embedded subtitles.
          return false;
        default:
          throw e;
      }
    }
  }

  public function initVideoCodec():Void {
    try {
      var result:Int = CppUtil.loadFunction("hx_ffmpeg_avcodec_init_video_codec", 1)(context);
      Error.handleError(result);

      // Only NOW are the video dimensions known.
      videoWidth = getVideoWidth();
      videoHeight = getVideoHeight();

      this.videoBitmapData = new BitmapData(videoWidth, videoHeight, true, 0x00000000);

      if (onInitVideoCodec != null) {
        onInitVideoCodec();
      }
    }
    catch (e:FFmpegError) {
      switch (e) {
        case ETryAgain:
          // We need to pass more data into the decoder.
          return;
        default:
          throw e;
      }
    }
  }

  public function decodeFrame():Bool {
    try {
      var result:Int = CppUtil.loadFunction("hx_ffmpeg_decode_frame", 3)(context, handleVideoFrame, handleAudioFrame);
      Error.handleError(result);

      var producedVideoFrame = (result == 1);

      return producedVideoFrame;
    }
    catch (e:FFmpegError) {
      switch (e) {
        case ETryAgain:
          // We need to pass more data into the decoder.
          return false;
        default:
          throw e;
      }
    }
  }

  /**
   * Every time a video frame is successfully decoded, this function is called.
   */
  function handleVideoFrame(data:BytesData) {
    trace('We have received a video frame!');

    trace('Received ${data.length} bytes of video data.');

    videoBitmapData.lock();
		videoBitmapData.setPixels(videoBitmapData.rect, data);
		videoBitmapData.unlock();

    if (onEmitVideoFrame != null) {
      onEmitVideoFrame();
    }
  }

  /**
   * Every time an audio frame is successfully decoded, this function is called.
   */
  function handleAudioFrame(data:BytesData) {
    trace('We have received an audio frame!');

    trace('Received ${data.length} bytes of audio data.'); 
  }

  function getVideoWidth():Int {
    trace('Fetching video width...');
    return CppUtil.loadFunction("hx_ffmpeg_get_video_width", 1)(context);
  }

  function getVideoHeight():Int {
    trace('Fetching video height...');
    return CppUtil.loadFunction("hx_ffmpeg_get_video_height", 1)(context);
  }

  /**
   * A debug utility function to dump detailed stream info to stdout.
   */
  public function dumpFormat():Void {
    CppUtil.loadFunction("hx_ffmpeg_av_dump_format", 1)(context);
  }
}
