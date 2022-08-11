package ffmpeg.openfl;

import openfl.media.Sound;
import openfl.media.SoundChannel;
import openfl.media.SoundTransform;
import openfl.events.SampleDataEvent;
import openfl.utils.ByteArray;

/**
 * Sound output designed specifically to be tied to an OpenFLBitmapVideo.
 */
class OpenFLVideoSound extends Sound {
    public var media(default, set):Media;

    public var playing(default, null):Bool = false;

    // TODO: Implement a standalone openfl.display.Sound wrapper for FLAC playback, etc.

    @:allow(ffmpeg.openfl.OpenFLBitmapVideo)
    private function new() {
        super();
    }

    public override function play(startTime:Float = 0.0, loops:Int = 0, sndTransform:SoundTransform = null):SoundChannel {
        trace('OpenFLVideoSound.play');
        playing = true;
        return super.play(startTime, loops, sndTransform);
    }

    function set_media(mediaIn:Media):Media {
        if (mediaIn != null) {
            enableCallback();
        } else {
            disableCallback();
        }
        return media = mediaIn;
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