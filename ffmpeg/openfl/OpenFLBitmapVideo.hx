package ffmpeg.openfl;

import openfl.media.Sound;
import ffmpeg.Error.FFmpegError;
import ffmpeg.Media;
import openfl.events.Event;
import openfl.display.Bitmap;

/**
 * Renders a video stream onto an OpenFL Bitmap.
 */
class OpenFLBitmapVideo extends Bitmap implements IVideo {
  private var media:Media;

  private var sound:OpenFLVideoSound;

  public var playing(default, null):Bool = true;

  public function new() {
    super();

    media = new Media();
    sound = new OpenFLVideoSound();
    sound.media = media;

    addEventListener(Event.ENTER_FRAME, onEnterFrame);
  }

  public function play() {
    playing = true;
    if (!sound.playing) {
        sound.play();
    }
  }

  public function open(url:String):Void {
    trace('Opening...');
    media.openInput(url, true, true);

    initCodec();

    sound.play();
  }

  public function attachMedia(media:Media):Void {
    this.media = media;
  }

  function initCodec():Void {
    trace('Initializing codec...');
    // DEBUG: Print video codec info
    media.dumpFormat();

    if (!media.hasVideo) {
      // throw "No video stream found in input.";
    } else {
      media.initVideoCodec();

      // DEBUG: Print video info
      trace('Init video codec success.');
      trace('videoWidth: ' + media.videoWidth);
      trace('videoHeight: ' + media.videoHeight);
    }

    if (!media.hasAudio) {} else {
      media.initAudioCodec();
      trace('Init audio codec success.');
    }

    trace('Starting playback threads...');
    media.startPlaybackThreads();
    trace('Playback threads started.');
  }

  function onEnterFrame(event:openfl.events.Event):Void {
    if (playing) {
      bitmapData = media.populateBitmapData();
      if (!sound.playing) {
        sound.play();
      }
    } else {
      // Do nothing.
    }
  }
}
