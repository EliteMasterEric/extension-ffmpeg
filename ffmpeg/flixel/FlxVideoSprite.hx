package ffmpeg;

import ffmpeg.Media;
#if flixel
import flixel.FlxSprite;

/**
 * This is a simple FlxSprite which plays a given video.
 * This is useful for no-effort intro videos and cutscenes.
 */
class FlxVideoSprite extends FlxSprite {
    private var __media:Media;

    public function new(fileUrl:String, x:Float = 0, y:Float = 0) {
        super(x, y);

        __media = new Media();
        __media.openInput(fileUrl);

        __media.fetchStreamInfo();
        
        __media.dumpFormat();

        __media.onInitVideoCodec = onInitVideoCodec;

        if (__media.hasVideo) {
            __media.initVideoCodec();
            __media.decodeFrame();
        } else {
            trace('[WARN] No video stream found in ' + fileUrl);
        }
    }

    function onInitVideoCodec() {
        this.pixels = __media.videoBitmapData;
    }
}
#end