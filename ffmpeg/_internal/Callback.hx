package ffmpeg._internal;

/**
 * A type which stores and manages callback functions.
 */
class Callback<T:haxe.Constraints.Function> {
    var parent:Dynamic;
    var callbackFns:Array<T>;

    public function new(parent:Dynamic) {
        this.parent = parent;
        this.callbackFns = [];
    }

    public function addCallback(fn:T):Void {
        callbackFns.push(fn);
    }

    public function removeCallback(fn:T):Void {
        var index = callbackFns.indexOf(fn);
        if (index != -1) {
            callbackFns.splice(index, 1);
        }
    }

    /**
     * Calls all registered callback functions.
     */
    public function call(...args:Dynamic):Void {
        for (cb in callbackFns) {
            Reflect.callMethod(parent, cb, [args]);
        }
    }
}