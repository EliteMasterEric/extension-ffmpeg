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

    public var playing(default, null):Bool = false;

    public function new() {
        super();

        media = new Media();
        sound = new OpenFLVideoSound();
        sound.media = media;
        
        addEventListener(Event.ENTER_FRAME, onEnterFrame);
    }

    public function play() {
        playing = true;
        sound.play();
    }

    public function open(url:String):Void {
        media.openInput(url);

        initCodec();
    }

    public function attachMedia(media:Media):Void {
        this.media = media;
    }

    function initCodec():Void {
        // DEBUG: Print video codec info
        media.dumpFormat();

        if (!media.hasVideo) {
            throw "No video stream found in input.";
        }

        media.initVideoCodec();

        // DEBUG: Print video info
        trace('Init video codec success.');
        trace('videoWidth: ' + media.videoWidth);
        trace('videoHeight: ' + media.videoHeight);

        media.initAudioCodec();
        trace('Init audio codec success.');
    }

    function onEnterFrame(event:openfl.events.Event):Void {
        if (playing) {
            media.decodeVideoFrame();
            media.fetchVideoFrame();
            bitmapData = media.populateBitmapData();
        } else {
            // Do nothing.
        }
    }
}
