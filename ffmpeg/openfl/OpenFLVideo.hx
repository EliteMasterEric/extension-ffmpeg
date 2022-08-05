package ffmpeg.openfl;

#if openfl
/**
 * An extension of `openfl.display.Video` which allows playback via FFmpeg.
 */
class OpenFLVideo extends openfl.display.Sprite {
    private var __media:ffmpeg.Media;
    
    public function new() {
        super();
    }

    public function attachMedia(media:ffmpeg.Media):Void {
        this.__media = media;
        this.__media.onEmitVideoFrame = onEmitVideoFrame;
    }

    private function onEmitVideoFrame():Void {
        trace('onEmitVideoFrame, begin bitmap fill');
        this.graphics.beginBitmapFill(this.__media.videoBitmapData);
    }
}
#end