/**
 * Multi-line comments for documentation.
 */

package;

import ffmpeg.Error.FFmpegError;
import sys.db.ResultSet;
import openfl.display.Bitmap;
import ffmpeg.Media;
import ffmpeg.Version;
import ffmpeg.openfl.OpenFLVideo;
import openfl.display.Sprite;

class Main extends Sprite {
  public function new() {
    super();

    printVersionInfo();

    // quit();
  }

  public function printVersionInfo():Void {
    trace('Welcome to extension-ffmpeg.');
    trace('libavcodec version: ' + Version.getLibavcodecVersion());
    trace('libavformat version: ' + Version.getLibavformatVersion());
    trace('libavutil version: ' + Version.getLibavutilVersion());
    trace('libavfilter version: ' + Version.getLibavfilterVersion());
    trace('libswresample version: ' + Version.getLibswresampleVersion());
    trace('libswscale version: ' + Version.getLibswscaleVersion());
    trace('avcodec_configuration: ' + Version.getAvcodecConfiguration());
    trace('avcodec_license: ' + Version.getAvcodecLicense());

    trace('===========================================================');

    // var filePath:String = "https://upload.wikimedia.org/wikipedia/commons/c/c5/Limes.webm";
    // var filePath:String = "file:assets/earth.mp4";
    // var filePath:String = "file:assets/Limes.webm";
    // var filePath:String = "file:assets/Limes.webm.srt";
    var filePath:String = "https://uploads.ungrounded.net/alternate/1528000/1528775_alternate_113347_r88.zip/assets/music/stressCutscene.mp4";

    trace('Opening file: ' + filePath);
    var media = new Media();

    media.openInput(filePath);
    trace('Open file success.');
    media.fetchStreamInfo();
    trace('Find stream info success.');
    media.dumpFormat();
    trace('Dump format success.');

    trace('===========================================================');

    trace('hasVideo: ' + media.hasVideo);
    trace('hasAudio: ' + media.hasAudio);
    trace('hasSubtitle: ' + media.hasSubtitle);

    trace('===========================================================');

    if (media.hasVideo) {
      media.initVideoCodec();
      trace('Init video codec success.');

      trace('videoWidth: ' + media.videoWidth);
      trace('videoHeight: ' + media.videoHeight);

      media.decodeFrame();

      var bitmap = new Bitmap(media.videoBitmapData);

      addChild(bitmap);

      trace('Decode video frame success.');

      var timer = new haxe.Timer(1000 / 30);
      timer.run = function() {
        // Decode frames until we emit a video frame.
        var result = false;
        do {
          try {
            result = media.decodeFrame();
          } catch (e:FFmpegError) {
            switch(e) {
              case EEndOfFile:
                trace('End of file.');
                timer.stop();
                break;
              default:
                throw e;
            }
          }
        } while (result);
      };
    }
  }

  public static function quit():Void {
    trace('Exiting...');
    Sys.exit(0);
  }
}
