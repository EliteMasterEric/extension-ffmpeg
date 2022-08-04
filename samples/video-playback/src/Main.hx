/**
 * Multi-line comments for documentation.
 */

package;

import ffmpeg.Media;
import ffmpeg.Version;
import openfl.display.Sprite;

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

    trace('===========================================================');

    var filePath:String = "file:assets/earth.mp4";

    trace('Opening file: ' + filePath);
    var media = new Media();
    media.openInput(filePath);
    trace('Open file success.');
    media.fetchStreamInfo();
    trace('Find stream info success.');
    media.dumpFormat();
  }

  public static function quit():Void {
    Sys.exit(0);
  }
}
