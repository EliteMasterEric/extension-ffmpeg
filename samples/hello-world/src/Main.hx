/**
 * Multi-line comments for documentation.
 */

package;

import ffmpeg.FFmpegUtil;

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
  }

  public static function quit():Void {
    Sys.exit(0);
  }
}
