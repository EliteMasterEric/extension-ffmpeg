/**
 * Multi-line comments for documentation.
 */

package;

import openfl.display.Sprite;
import ffmpeg.Version;

class Main extends Sprite {
  public function new () {
		super ();

    printVersionInfo();

    quit();
	}

  public static function printVersionInfo():Void {
    trace('Welcome to extension-ffmpeg.');
    trace('libavcodec version: ' + Version.getLibavcodecVersion());
    trace('libavformat version: ' + Version.getLibavformatVersion());
    trace('libavutil version: ' + Version.getLibavutilVersion());
    trace('libavfilter version: ' + Version.getLibavfilterVersion());
    trace('libswresample version: ' + Version.getLibswresampleVersion());
    trace('libswscale version: ' + Version.getLibswscaleVersion());
    trace('avcodec_configuration: ' + Version.getAvcodecConfiguration());
    trace('avcodec_license: ' + Version.getAvcodecLicense());
  }

  public static function quit():Void {
    Sys.exit(0);
  }
}
