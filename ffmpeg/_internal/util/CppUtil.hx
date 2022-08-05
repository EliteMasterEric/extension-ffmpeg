package ffmpeg._internal.util;

import cpp.Lib;

class CppUtil {
  static final _functionCache:Map<String, Dynamic> = [];

  /**
   * Loads a C++ function from the library via the CFFI interface,
   * caches it for future use, and returns it to be called.
   */
  public static function loadFunction(name:String, argCount:Int):Dynamic {
    if (_functionCache.exists(name)) {
      return _functionCache.get(name);
    }

    var func = Lib.load("extension-ffmpeg", name, argCount);
    if (func == null) {
      func = function() {
        throw "[extension-ffmpeg] Function '" + name + "' with arg count " + argCount + " could not be resolved.";
      };
    }
    _functionCache.set(name, func);

    return func;
  }
}
