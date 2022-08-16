package ffmpeg;

import ffmpeg._internal.util.CppUtil;
import ffmpeg._internal.Callback;
import ffmpeg.Error;
import ffmpeg.Error.FFmpegError;
import haxe.io.BytesData;
import haxe.io.Bytes;
// import hx.concurrent.lock.RLock;
import openfl.display.BitmapData;
import openfl.utils.ByteArray;

/**
 * Represents a media file accessible by FFmpeg.
 * Allows output of audio or video to a buffer.
 */
class Media {
  /**
   * `generateSound()` must output at least this many bytes.
   */
  public static final REQUIRED_SAMPLES = 2048 * 4 * 2;

  /**
   * Stores a pointer to the `FFmpegContext` in C++.
   * Pass it into any C++ functions to ensure the correct media is manipulated.
   */
  private var context:Dynamic;

  /**
   * Whether the associated media file has a video stream.
   * Enabled only once the video codec has been initialized,
   * and the media is ready to play video.
   */
  public var hasVideo(default, null):Bool = false;

  /**
   * The audio codec has been initialized, and the media is ready to play audio.
   */
  public var videoReady(default, null):Bool = false;

  /**
   * Whether the associated media file has an audio stream.
   * Enabled only once the audio stream data has been fetched,
   * and the codec is ready to initialize.
   */
  public var hasAudio(default, null):Bool = false;

  /**
   * The audio codec has been initialized, and the media is ready to play audio.
   */
  public var audioReady(default, null):Bool = false;

  /**
   * The width of the video, in pixels.
   */
  public var videoWidth(get, null):Int;

  /**
   * The height of the video, in pixels.
   */
  public var videoHeight(get, null):Int;

  //
  // CALLBACK FUNCTIONS
  //

  /**
   * Called when the video codec has initialized successfully.
   */
  public var onInitVideoCodec(default, null):Callback<Void->Void>;

  /**
   * Called when the audio codec has initialized successfully.
   */
  public var onInitAudioCodec(default, null):Callback<Void->Void>;

  /**
   * Called when the media stream has completed (i.e. the end of the video has been reached).
   */
  public var onComplete(default, null):Callback<Void->Void>;

  // Whether the media has been accessed for basic information.
  private var mediaLoaded:Bool = false;
  // Whether the media has been accessed for advanced codec information.
  private var streamInfoLoaded:Bool = false;

  // Byte array containing the video frame data.
  private var videoFrameBuffer(default, null):ByteArray;

  // Byte array containing the audio frame data.
  private var soundOutputBuffer(default, null):ByteArray;

  #if cpp
  /**
   * The `soundOutputBuffer` needs a mutex, to prevent data from being overridden while it is being read.
   */
  private var soundOutputBufferMutex = new sys.thread.Mutex();
  #end

  #if openfl
  // A BitmapData object containing the current video frame.
  private var videoBitmapData(default, null):BitmapData;
  #end

  public function new() {
    context = buildContext();

    onInitVideoCodec = new Callback<Void->Void>(this);
    onInitAudioCodec = new Callback<Void->Void>(this);
    onComplete = new Callback<Void->Void>(this);
  }

  /**
   * Build an FFmpegContext object, used to hold information about the media.
   * @throws FFmpegError if the context could not be initialized.
   */
  static final VIDEO_QUEUE_SIZE = 48;
  static final AUDIO_QUEUE_SIZE = 32;
  private function buildContext():Dynamic {
    trace('Building FFmpegContext...');
    var result = CppUtil.loadFunction("hx_ffmpeg_init_ffmpegcontext", 2)(VIDEO_QUEUE_SIZE, AUDIO_QUEUE_SIZE);
    trace('Context built.');
    return result;
  }

  /**
   * Open a media file via URL. Any protocol may be used (including `file:` and `https:`).
   * 
   * @param url the URL of the media file.
   * @throws FFmpegError if the media file could not be opened.
   */
  public function openInput(url:String, video:Bool = true, audio:Bool = true):Void {
    trace('Opening media file...');
    var result:Int = CppUtil.loadFunction("hx_ffmpeg_avformat_open_input", 2)(context, url);
    Error.handleError(result);

    trace('Media file opened');

    mediaLoaded = true;

    fetchStreamInfo(video, audio);
  }

  /**
   * Fetches info for each stream type from the media file.
   * Can only be called if the media is loaded.
   */
  function fetchStreamInfo(video:Bool = true, audio:Bool = true):Void {
    if (!mediaLoaded)
      return;

    trace('Fetching stream info...');

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
   * Can only be called if the stream info is loaded.
   */
  function fetchVideoStream():Bool {
    try {
      if (!streamInfoLoaded)
        return false;

      trace('Fetching video stream...');

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
   * Can only be called if the stream info is loaded.
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
   * Can only be called if the video stream is found.
   */
  public function initVideoCodec():Void {
    try {
      if (!hasVideo)
        return;

      trace('Starting video codec...');

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_avcodec_init_video_codec", 1)(context);
      Error.handleError(result);

      // Success!
      videoReady = true;

      // Perform additional allocation.
      videoFrameBuffer = new ByteArray(videoWidth * videoHeight * 4);

      if (onInitVideoCodec != null) {
        onInitVideoCodec.call();
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
   * Can only be called if the audio stream is found.
   */
  public function initAudioCodec():Void {
    try {
      if (!hasAudio)
        return;

      var result:Int = CppUtil.loadFunction("hx_ffmpeg_avcodec_init_audio_codec", 1)(context);
      Error.handleError(result);

      // Success!
      audioReady = true;

      // Perform additional allocation.
      soundOutputBuffer = new ByteArray();

      if (onInitAudioCodec != null) {
        onInitAudioCodec.call();
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

  public function startPlaybackThreads():Void {
    if (!hasVideo && !hasAudio)
      throw "No streams initialized for this media file.";

    // Start the decoding thread.
    startDecodeThread();

    if (hasVideo) {
      startVideoThread();
    }
    if (hasAudio) {
      // startAudioThread();
    }
  }

  function startDecodeThread():Void {
    var result:Int = CppUtil.loadFunction("hx_ffmpeg_start_decode_thread", 1)(context);
    Error.handleError(result);
  }

  function startVideoThread():Void {
    // Start the video playback thread.
    var result = CppUtil.loadFunction("hx_ffmpeg_start_video_thread", 2)(context, handleVideoFrame);
    Error.handleError(result);
  }

  function startAudioThread():Void {
    // Start the audio playback thread.
    var result = CppUtil.loadFunction("hx_ffmpeg_start_audio_thread", 2)(context, handleAudioFrame);
    Error.handleError(result);
  }

  /**
   * Callback function which receives a video frame from the video thread for playback.
   * Should be called whenever there is a new frame to display.
   */
  var lastStamp:Float = 0;

  function handleVideoFrame(data:BytesData):Void {
    // Set the video frame buffer to the newly provided frame data.
    var newStamp = haxe.Timer.stamp();
    var diffStamp = newStamp - lastStamp;
    lastStamp = newStamp;
    // trace('[MEDIA] Received video frame (rate: ' + 1/diffStamp + ')');

    // Implicit cast.
    videoFrameBuffer = data;

    #if openfl
    // populateBitmapData();
    #end
  }

  #if openfl
  /**
   * Populate a BitmapData object with the next video frame and return it.
   * We reuse the BitmapData between calls to avoid allocating a new object each time.
   *
   * Only available if the `openfl` library is included.
   */
  public function populateBitmapData():BitmapData {
    // trace('[MEDIA] Populating bitmap data...');

    // Initialize the bitmap data if necessary.
    if (videoBitmapData == null) {
      videoBitmapData = new BitmapData(videoWidth, videoHeight, true, 0x00000000);
    }

    if (videoFrameBuffer.length < (videoWidth * videoHeight * 4)) {
      // trace('[MEDIA] Video frame buffer is too small, cannot render.');
    } else {
      // Copy the frame data into the bitmap.
      videoFrameBuffer.position = 0;
      videoBitmapData.lock();
      videoBitmapData.setPixels(videoBitmapData.rect, videoFrameBuffer);
      videoBitmapData.unlock();
    }
    return videoBitmapData;
  }
  #end

  /**
   * Callback function which receives an audio frame from the audio thread for playback.
   * Should be called whenever there is a new frame to play.
   */
  function handleAudioFrame(data:BytesData):Void {
    // trace('[MEDIA] Received audio frame...');

    soundOutputBufferMutex.acquire();

    // Append the new data to the end of the buffer.
    soundOutputBuffer.position = soundOutputBuffer.length;
    soundOutputBuffer.writeBytes(data, 0, data.length);

    soundOutputBufferMutex.release();
  }

  /**
   * Write bytes from the audio buffer to the provided array for use by the sound output.
   * If too much data is stored in the buffer, excess bytes will be kept for later.
   * If not enough data is stored in the buffer, the output buffer will be filled with silence (zeros).
   */
  public function generateSound(data:ByteArray):Void {
    if (data == null)
      throw 'The output buffer cannot be null.';
    
    // Extend the buffer if needed.
    if (data.length < REQUIRED_SAMPLES)
      data.length = REQUIRED_SAMPLES;

    var dataPointer:BytesData = data;
    var result = CppUtil.loadFunction("hx_ffmpeg_generate_audio", 3)(context, dataPointer, REQUIRED_SAMPLES);
    Error.handleError(result);
  }

  function generateSoundOld(data:ByteArray):Void {
    soundOutputBufferMutex.acquire();

    var bytesAvailable, bytesToWrite, blanksToWrite, bytesRemaining:Int;

    if (!audioReady) {
      // Audio codec is not initialized so data will be empty.
      bytesAvailable = 0;
      bytesToWrite = 0;
      blanksToWrite = REQUIRED_SAMPLES;
      bytesRemaining = 0;
    } else {
      // The audio codec is working, so we can write data to the output buffer.
      bytesAvailable = soundOutputBuffer.length;
      bytesToWrite = Std.int(Math.min(REQUIRED_SAMPLES, bytesAvailable));
      blanksToWrite = REQUIRED_SAMPLES - bytesToWrite;
      bytesRemaining = bytesAvailable - bytesToWrite;
    }

    if (bytesToWrite > 0) {
      data.writeBytes(soundOutputBuffer, 0, bytesToWrite);
    }
    if (blanksToWrite > 0) {
      trace('[MEDIA] Audio buffer is too small, ${blanksToWrite} bytes of silence.');
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
    } else {
      // Replace the buffer with an empty one. This is easier than writing zeros.
      soundOutputBuffer = new ByteArray();
    }

    soundOutputBufferMutex.release();
  }

  /**
   * A debug utility function to dump detailed stream info to stdout.
   */
  public function dumpFormat():Void {
    CppUtil.loadFunction("hx_ffmpeg_av_dump_format", 1)(context);
  }

  /**
   * DEBUG FUNCTION: Print a single pixel from the frame buffer to the console.
   * Prints the RGBA value as a hex value.
   */
  public function printPixel(x:Int = 0, y:Int = 0):Void {
    videoFrameBuffer.position = (4 * (y * videoWidth + x));

    trace(StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2) + ':' + StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2) + ':'
      + StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2) + ':' + StringTools.hex(videoFrameBuffer.readUnsignedByte(), 2));
  }
}

// Represents a class which can manipulate an underlying media object.
// Contains functions common to both video and audio.
interface IMedia {
  /**
   * Attempt to load a media file from a given URL.
   *
   * @param url The location of the media file to load. Supports local or remote (`https`) files.
   * @throws FFmpegError if the media file could not be loaded.
   */
  public function open(url:String):Void;

  /**
   * Attempt to start media playback.
   * This will start playback from the beginning if playback has not started, or resume playback if it has.
   */
  public function play():Void;

  /**
   * Attempt to stop media playback.
   * Later calling `play()` will restart playback from the beginning.
   * This will do nothing if the media is not currently playing.
   */
  // public function stop():Void;
  /**
   * Attempt to restart media playback.
   * This will reset the position to the beginning and start playback.
   */
  // public function restart():Void;
  /**
   * Attempt to pause media playback.
   * Later calling `play()` will resume playback from the current position.
   * This will do nothing if the media is not currently playing.
   */
  // public function pause():Void;
  /**
   * Attempt to resume media playback.
   * This will do nothing if the media has not previously started by calling `play()`.
   */
  // public function resume():Void;
  /**
   * Toggle pause on media playback.
   * If the media is paused, it will play, and if the media is playing, it will pause.
   */
  // public function togglePause():Void;
  /**
   * Attempt to seek to the specified position, a specific time in seconds.
   * This will do nothing if the media is not currently playing.
   */
  // public function setPosition(time:Float):Void;
  /**
   * Attempt to seek to the specified position, a percentage of the media's duration.
   * This will do nothing if the media is not currently playing.
   *
   * @param percent A percentage value between 0 and 1.
   */
  // public function setPositionPercent(percent:Float):Void;
  /**
   * Retrieve the duration of the media, in seconds.
   */
  // public function getDuration():Float;
  /**
   * Retrieve the current position in the media, in seconds.
  **/
  // public function getPosition():Float;
  /**
   * Retrieve the current position in the media, as a percentage of the media's duration.
   *
   * @return A percentage value between 0 and 1.
   */
  // public function getPositionPercent():Float;
  /**
   * Whether the media should immediately restart playback upon completion.
   */
  // public var looped(get, set):Bool;
}

// Represents a class which can manipulate an underlying video media object.
// Contains audio-specific functions.
interface IAudio extends IMedia {
  /**
   * The volume of the media, between [0, 1].
   */
  // public var volume(get, set):Float;
  /**
   * Whether the media is muted.
   * Toggling this value prevents audio playback,
   * but does not affect the underlying `volume` setting.
   */
  // public var muted(get, set):Bool;
}

// Represents a class which can manipulate an underlying video media object.
// Contains video-specific functions.
interface IVideo extends IAudio {
  /**
   * The width of the output video, in pixels, during playback.
   */
  // public var videoWidth(get, set):Int;
  /**
   * The height of the output video, in pixels, during playback.
   */
  // public var videoHeight(get, set):Int;
}
