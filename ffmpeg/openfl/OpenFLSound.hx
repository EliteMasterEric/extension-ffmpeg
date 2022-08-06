package ffmpeg.openfl;

import openfl.media.SoundChannel;
import openfl.utils.ByteArray;
import openfl.media.Sound;
import openfl.media.SoundTransform;
import openfl.events.Event;
import openfl.events.SampleDataEvent;

class OpenFLSound extends Sound {
  private var __media:Media;

  public var playing:Bool = true;

  public function new() {
    super();

    this.__media = new Media();
  }

  public function open(url:String):Void {
    this.__media.openInput(url);

    initCodec();
  }

  public function attachMedia(media:Media):Void {
    this.__media = media;
  }

  function initCodec():Void {
    // DEBUG: Print media codec info
    this.__media.dumpFormat();

    if (!this.__media.hasAudio) {
      throw "No audio stream found in input.";
    }

    this.__media.initVideoCodec();
    this.__media.initAudioCodec();

    // DEBUG: Print audio info
    trace('Init audio codec success.');
  }

  function decodeAudioFrame():Void {
    // Command FFmpeg to decode until a video frame is available.
    var result:Int = Media.FrameType.Again;
    do {
      trace('Decoding frame...');
      result = this.__media.decodeFrame();
      trace(result);
    }
    while (result != Media.FrameType.Audio);
  }

  public override function play(startTime:Float = 0.0, loops:Int = 0, sndTransform:SoundTransform = null):SoundChannel {
    addEventListener(SampleDataEvent.SAMPLE_DATA, generateSound);
    return super.play(startTime, loops, sndTransform);
  }

  function generateSound(event:SampleDataEvent):Void {

    if (playing) {
        decodeAudioFrame();
    }

    if (event.data == null)
      event.data = new ByteArray();

    var outputLength = this.__media.soundOutputBuffer.length;
    var bytesToWrite = Std.int(Math.min(REQUIRED_SAMPLES, outputLength));
    var blankLength = REQUIRED_SAMPLES - outputLength;

    trace('Writing ' + bytesToWrite + ' bytes + ' + blankLength + ' blank bytes.');

    if (blankLength > 0) {
      event.data.writeBytes(BLANK_BYTES, 0, blankLength);
    }

    if (bytesToWrite > 0) {
      event.data.writeBytes(this.__media.soundOutputBuffer, 0, bytesToWrite);
    }

    if (bytesToWrite < outputLength) {
        var excessBytes = new ByteArray();
        excessBytes.writeBytes(this.__media.soundOutputBuffer, bytesToWrite, outputLength - bytesToWrite);
        this.__media.soundOutputBuffer = excessBytes;
    } else {
        this.__media.soundOutputBuffer.clear();
    }
  }
}
