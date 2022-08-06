# Documentation and resources
- How to write a video player: http://dranger.com/ffmpeg/
- Compilation on RT: https://trac.ffmpeg.org/wiki/CompilationGuide/WinRT#PrerequisitesandFirstTimeSetupInstructions
- MS on Visual Studio command prompt: https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170
- List of options: https://stackoverflow.com/questions/24849129/compile-ffmpeg-without-most-codecs
- https://stackoverflow.com/questions/8403229/ffmpeg-api-compiling-with-specific-formats
- https://stackoverflow.com/questions/50622515/what-encoders-decoders-muxers-demuxers-parsers-do-i-need-to-enable-in-ffmpeg-for
- http://howto-pages.org/ffmpeg/#build
- https://stackoverflow.com/questions/40061101/how-to-access-windows-enviroment-variables-on-msys
- https://riptutorial.com/ffmpeg/example/30960/find-a-stream
- https://sodocumentation.net/ffmpeg/topic/10090/decoding

## CFFI Libraries
- https://github.com/MattTuttle/Audaxe/blob/fa327e4ac959f5bd528bff626ccb005f395ad2f3/project/common/ExternalInterface.cpp
- https://github.com/soywiz-archive/haxe-openfl-ffmpeg/blob/42ec9f648a7810b1f0eb263a45d560d457ad4404/libs.src/build_ffmpeg.sh

/**
 * Additional notes on Haxe CFFI
 * - Use `alloc_string()` to create a Haxe string from a CPP string.
 * - Use `alloc_int()` to create a Haxe int from a CPP int.
 * - Use `val_string()` to get a CPP string from a Haxe string.
 * - Use `val_int()` to get a CPP int from a Haxe int.
 */

## Bytes

- `openfl.utils.ByteArray` is an abstract, it provides simple read/write utilities and conversion functions to `openfl.utils.ByteArrayData`.
    - To `ByteArrayData`: No conversion needed.
    - To `Bytes`: Available as implicit cast.
    - To `BytesData`: Available as implicit cast.
- `openfl.utils.ByteArrayData` is implemented differently on diffrent platforms. It provides position-based read-write operations.
    - Use `ByteArray` instead, it provides useful type conversions and casting operations.
- `haxe.io.Bytes` is a class which wraps a `haxe.io.BytesData`. It acts as a simple array accessor for the underlying `BytesData`.
    - To `ByteArray`: `ByteArray.fromBytes(value)`
- `haxe.io.BytesData` is a typedef, which varies by platform. For example, on CPP, it is a `Array<cpp.UInt8>`.
    - To `ByteArray`: `ByteArray.fromBytesData(value)`

## Resolving common errors:
- Symbol defined multiple times - Make sure #define is called only once, check if something is undefined before defining it.
- LNK2019: unresolved external symbol - Make sure the `.c` file that includes the file is included in the `Build.xml` file.
