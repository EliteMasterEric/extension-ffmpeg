# Documentation and resources
These are useful links I used when building this library.

## FFmpeg
- How to write a video player in less than 1000 lines: http://dranger.com/ffmpeg/
- FFmpeg libav tutorial: https://github.com/leandromoreira/ffmpeg-libav-tutorial
- FFmpeg configure options: https://stackoverflow.com/questions/24849129/compile-ffmpeg-without-most-codecs
- FFmpeg modern frame decoding: https://sodocumentation.net/ffmpeg/topic/10090/decoding

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
- LNK2019: unresolved external symbol - Make sure the library that implements the function is included in the `Build.xml` file.

## FFMPEG Basics
The AVFormatContext is the abstraction for the format of the media file, aka container (ex: MKV, MP4, Webm, TS). The AVStream represents each type of data for a given format (ex: audio, video, subtitle, metadata). The AVPacket is a slice of compressed data obtained from the AVStream that can be decoded by an AVCodec (ex: av1, h264, vp9, hevc) generating a raw data called AVFrame.

### PTS
For a fps=60/1 and timebase=1/60000 each PTS will increase timescale / fps = 1000 therefore the PTS real time for each frame could be (supposing it started at 0):

frame=0, PTS = 0, PTS_TIME = 0
frame=1, PTS = 1000, PTS_TIME = PTS * timebase = 0.016
frame=2, PTS = 2000, PTS_TIME = PTS * timebase = 0.033
