/**
 * Multi-line comments for documentation.
 */

package;

import openfl.display.Sprite;
import ffmpeg.FFmpegUtil;
import ffmpeg.Video;

class Main extends Sprite {
  public function new () {
		super ();

    printVersionInfo();

    quit();
	}

  public static function printVersionInfo():Void {
    trace('Welcome to extension-ffmpeg.');
    trace('libavcodec version: ' + FFmpegUtil.getLibavcodecVersion());
    trace('libavformat version: ' + FFmpegUtil.getLibavformatVersion());
    trace('libavutil version: ' + FFmpegUtil.getLibavutilVersion());
    trace('libavfilter version: ' + FFmpegUtil.getLibavfilterVersion());
    trace('libswresample version: ' + FFmpegUtil.getLibswresampleVersion());
    trace('libswscale version: ' + FFmpegUtil.getLibswscaleVersion());
    trace('avcodec_configuration: ' + FFmpegUtil.getAvcodecConfiguration());
    trace('avcodec_license: ' + FFmpegUtil.getAvcodecLicense());

    var context = Video.buildContext();
    trace('Was context defined? ' + (context != null));
    var resultA = Video.openFile(context, "assets/earth.mp4");
    trace('Open file success? ' + (resultA));
    var resultB = Video.streamInfo(context);
    trace('Find stream info success? ' + (resultB));

  }

  public static function quit():Void {
    Sys.exit(0);
  }
}
