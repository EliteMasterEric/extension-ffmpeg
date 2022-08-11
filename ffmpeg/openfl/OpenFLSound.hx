package ffmpeg.openfl;

import openfl.media.Sound;
import openfl.media.SoundChannel;
import openfl.media.SoundTransform;
import openfl.events.SampleDataEvent;
import openfl.utils.ByteArray;

/**
 * Sound output designed specifically to be tied to an OpenFLBitmapVideo.
 */
class OpenFLSound extends Sound {
    private var media:Media;

    public var playing(default, null):Bool = false;

    public function new() {
        super();
        media = new Media();
    }

    public function open(url:String):Void {
      trace('Opening...');
      media.openInput(url, false, true);
  
      initCodec();
    }

    function initCodec():Void {
      trace('Initializing codec...');
      // DEBUG: Print video codec info
      media.dumpFormat();

      if (!media.hasAudio) {} else {
        media.initAudioCodec();
        trace('Init audio codec success.');
      }
  
      trace('Starting playback threads...');
      media.startPlaybackThreads();
      trace('Playback threads started.');
    }

    public override function play(startTime:Float = 0.0, loops:Int = 0, sndTransform:SoundTransform = null):SoundChannel {
        trace('OpenFLSound.play');
        playing = true;
        enableCallback();
        return super.play(startTime, loops, sndTransform);
    }

    function enableCallback():Void {
        addEventListener(SampleDataEvent.SAMPLE_DATA, generateSound);
    }

    function disableCallback():Void {
        removeEventListener(SampleDataEvent.SAMPLE_DATA, generateSound);
    }

    function generateSound(event:SampleDataEvent):Void {
        // trace('Generating sound...');
        if (event.data == null)
            event.data = new ByteArray();

        media.generateSound(event.data);
    }
}