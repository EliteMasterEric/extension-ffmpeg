package ffmpeg;

import ffmpeg._internal.util.CppUtil;
import ffmpeg.Error;
import ffmpeg.Error.FFmpegError;
import haxe.io.BytesData;
import openfl.display.BitmapData;
import openfl.utils.ByteArray;

/**
 * Represents a media file accessible by FFmpeg.
 */
class Media {
  /**
   * `generateSound()` must output at least this many bytes.
   */
  static final REQUIRED_SAMPLES = 2048 * 4 * 2;

  private var context:Dynamic;

  /**
   * Whether the associated media file has a video stream.
   */
  public var hasVideo(default, null):Bool = false;
  public var videoCodecStarted(default, null):Bool = false;

  /**
   * Whether the associated media file has an audio stream.
   */   
  public var hasAudio(default, null):Bool = false;
  public var audioCodecStarted(default, null):Bool = false;

  /**
   * The width of the video, in pixels.
   */
  public var videoWidth(get, null):Int;

  /**
   * The height of the video, in pixels.
   */
  public var videoHeight(get, null):Int;

  public var videoBitmapData(default, null):BitmapData;
  
  // 
  // CALLBACK FUNCTIONS
  // 

  public var onInitVideoCodec:Void->Void = null;
  public var onInitAudioCodec:Void->Void = null;

  private var mediaLoaded:Bool = false;
  private var streamInfoLoaded:Bool = false;

  /**
   * The byte array which video frame data is written to.
   * Holds a single video frame.
   */
  private var videoFrameBuffer:ByteArray;
  /**
   * The byte array which audio frame data is written to.
   * Dynamically expands to hold audio frames until they are output.
   */
  private var soundOutputBuffer:ByteArray;

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
   * 
   * @param url the URL of the media file.
   * @throws FFmpegError if the media file could not be opened.
   */
  public function openInput(url:String, video:Bool = true, audio:Bool = true):Void {
    var result:Int = CppUtil.loadFunction("hx_ffmpeg_avformat_open_input", 2)(context, url);
    Error.handleError(result);

    mediaLoaded = true;

    fetchStreamInfo(video, audio);
  }

  /**
   * Fetches info for each stream type from the media file.
   */
  function fetchStreamInfo(video:Bool = true, audio:Bool = true):Void {
    if (!mediaLoaded)
      return;

    var result:Int = CppUtil.loadFunction("hx_ffmpeg_avformat_find_stream_info", 1)(context);
    Error.handleError(result);

    streamInfoLoaded = true;

    if (video) {
      hasVideo = fetchVideoStream();
    }
    if (audio) {
      hasAudio = fetchAudioStream();
    }
  }

  /**
   * Search the media file for the best video stream.
   */
  function fetchVideoStream():Bool {
    try {
      if (!streamInfoLoaded)
        return false;

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_av_find_best_video_stream", 1)(context);
      Error.handleError(result);

      // Return true if a video stream was found.
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
      if (!streamInfoLoaded)
        return false;

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_av_find_best_audio_stream", 1)(context);
      Error.handleError(result);

      // Return true if an audio stream was found.
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

  function get_videoWidth():Int {
    if (!hasVideo)
      return -1;
    return CppUtil.loadFunction("hx_ffmpeg_get_video_width", 1)(context);
  }

  function get_videoHeight():Int {
    if (!hasVideo)
      return -1;
    return CppUtil.loadFunction("hx_ffmpeg_get_video_height", 1)(context);
  }

  /**
   * Initialize the video codec.
   */
  public function initVideoCodec():Void {
    try {
      if (!hasVideo)
        return;

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_avcodec_init_video_codec", 1)(context);
      Error.handleError(result);

      // Success!
      videoCodecStarted = true;

      // Perform additional allocation.
      videoFrameBuffer = new ByteArray(videoWidth * videoHeight * 4);
      videoBitmapData = new BitmapData(videoWidth, videoHeight, true, 0x00000000);

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

  /**
   * Initialize the audio codec.
   */
  public function initAudioCodec():Void {
    try {
      if (!hasAudio)
        return;

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_avcodec_init_audio_codec", 1)(context);
      Error.handleError(result);

      // Success!
      audioCodecStarted = true;

      // Perform additional allocation.
      soundOutputBuffer = new ByteArray();

      if (onInitAudioCodec != null) {
        onInitAudioCodec();
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

  static final FRAME_AGAIN = 0;
  static final FRAME_VIDEO = 1;
  static final FRAME_AUDIO = 2;

  /**
   * Decode a single packet from the media file.
   * The packet may contain partial or complete video or audio data.
   */
  public function decodeFrame():Int {
    try {
      if (!videoCodecStarted && !audioCodecStarted)
        throw EStreamNotFound;

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_decode_frame", 2)(context, handleAudioFrame);
      Error.handleError(result);

      return result;
    }
    catch (e:FFmpegError) {
      switch (e) {
        case ETryAgain:
          // We need to pass more data into the decoder.
          return FRAME_AGAIN;
        default:
          throw e;
      }
    }
  }

  /**
   * Continually decode packets from the media file until the desired data is available.
   */
  function decodeUntil(targetFrameType:Int) {
    var result:Int = FRAME_AGAIN;

    do {
      try {
        result = decodeFrame();
      } catch (e:FFmpegError) {
        switch (e) {
          case EEndOfFile:
            // We've reached the end of the file.
            return;
          case ETryAgain:
            // We need to pass more data into the decoder.
            result = 0;
          default:
            throw e;
        }
      }
    } while (result != targetFrameType);
  }

  /**
   * Continually decode packets from the media file until a new video frame is available.
   */
  public inline function decodeVideoFrame():Void {
    decodeUntil(FRAME_VIDEO);
  }

  /**
   * Continually decode packets from the media file until new audio data is available.
   */
  public inline function decodeAudioFrame():Void {
    decodeUntil(FRAME_AUDIO);
  }

  /**
   * Manually fetch the next video frame, and place the result in the `videoBitmapData` buffer.
   */
  public function fetchVideoFrame():BitmapData {
    try {
      if (!videoCodecStarted)
        throw EStreamNotFound;

      videoFrameBuffer.position = 0;
      var inBuffer:BytesData = videoFrameBuffer; // Cast to BytesData.
      var result:Int = CppUtil.loadFunction("hx_ffmpeg_emit_video_frame", 2)(context, inBuffer);
      Error.handleError(result);
      
      // Copy the frame data into the bitmap.
      videoFrameBuffer.position = 0;
      videoBitmapData.lock();
      videoBitmapData.setPixels(videoBitmapData.rect, videoFrameBuffer);
      videoBitmapData.unlock();

      return videoBitmapData;
    } catch (e:FFmpegError) {
      switch (e) {
        default:
          throw e;
      }
    }
  }

  
  public function printPixel(x:Int = 0, y:Int = 0):Void {
    videoFrameBuffer.position = (4 * (y * videoWidth + x));

    trace(StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2) + ':'
        + StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2) + ':'
        + StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2) + ':'
        + StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2));
  }

  /**
   * Manually clear the video frame buffer.
   * @param color The color to fill with. Defaults to transparent.
   */
  public function clearVideoFrame(?color:Int=0x00000000):Void {
    videoBitmapData.fillRect(videoBitmapData.rect, color);
  }

  /**
   * Automatically handle the next audio frame, and place the result in the `audioBuffer` buffer.
   * @param data The audio data from the decoder. Basically an `Array<cpp.UInt8>`.
   */
  function handleAudioFrame(data:BytesData):Void {
    // Append the new data to the end of the buffer.
    soundOutputBuffer.position = soundOutputBuffer.length;
    soundOutputBuffer.writeBytes(data, 0, data.length);

    trace('Appended ' + data.length + ' bytes to the sound output buffer (now ' + soundOutputBuffer.length + ' bytes).');
  }

  /**
   * Write bytes from the audio buffer to the provided array for use by the sound output.
   * If too much data is stored in the buffer, excess bytes will be kept for later.
   * If not enough data is stored in the buffer, the output buffer will be filled with silence (zeros).
   */
  public function generateSound(data:ByteArray):Void {
    if (data == null)
      throw 'The output buffer cannot be null.';

    trace('Writing bytes to sound output buffer...');
    var bytesAvailable:Int = soundOutputBuffer.length; 
    var bytesToWrite:Int = Std.int(Math.min(REQUIRED_SAMPLES, bytesAvailable));
    var blanksToWrite:Int = REQUIRED_SAMPLES - bytesToWrite;
    var bytesRemaining:Int = bytesAvailable - bytesToWrite;

    trace('Writing ' + bytesToWrite + ' bytes to the output buffer, and ' + blanksToWrite + ' blank bytes.');
    if (bytesToWrite > 0) {
      data.writeBytes(soundOutputBuffer, 0, bytesToWrite);
    }
    if (blanksToWrite > 0) {
      var count = 0;
      while (count < blanksToWrite) {
        data.writeByte(0);
        count++;
      }
    }
    if (bytesRemaining > 0) {
      // Replace the buffer with one containing only the remaining bytes.
      var leftoverBytes = new ByteArray();
      leftoverBytes.writeBytes(soundOutputBuffer, bytesToWrite, bytesRemaining);
      soundOutputBuffer = leftoverBytes;
    } {
      // Replace the buffer with an empty one.
      soundOutputBuffer = new ByteArray();
    }
  }

  /**
   * A debug utility function to dump detailed stream info to stdout.
   */
  public function dumpFormat():Void {
    CppUtil.loadFunction("hx_ffmpeg_av_dump_format", 1)(context);
  }
}

interface IMedia {
  // public function play():Void;
  // public function pause():Void;
  // public function stop():Void;
  // public function seek(position:Float):Void;
  // public function getPosition():Float;
  // public function getDuration():Float;
  // public function getVolume():Float;
  // public function setVolume(volume:Float):Void;
  // public function getMuted():Bool;
  // public function setMuted(muted:Bool):Void;
  // public function getLoop():Bool;
  // public function setLoop(loop:Bool):Void;
}

interface IVideo extends IMedia {

}

interface IAudio extends IMedia {
  
}