# Compiles FFmpeg for 32-bit Linux
# https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu

# Compilation flags for FFmpeg
FLAGS=""

# Cross-compilation flags.
# To successfully build for Linux, you must have an i686 (x32 bit only) build of the library.
FLAGS="$FLAGS --cc='cc -m32'"
FLAGS="$FLAGS --enable-static"

# Disable the features we don't want or use to reduce compile times.
FLAGS="$FLAGS --disable-doc"
FLAGS="$FLAGS --disable-programs"

# Direct build logs to a file.
FLAGS="$FLAGS --logfile=../build.log"
# Upgrade license to LGPLv3, allowing use of some ApacheV2 licensed dependencies.
FLAGS="$FLAGS --enable-version3"

pushd ffmpeg
./configure $FLAGS && make
popd

# Export the necessary binaries.
OUTPUT_DIR=../lib.build/Linux32
mkdir -p $OUTPUT_DIR
cp -f ffmpeg/libavcodec/libavcodec.a $OUTPUT_DIR/libavcodec.a
cp -f ffmpeg/libavdevice/libavdevice.a $OUTPUT_DIR/libavdevice.a
cp -f ffmpeg/libavfilter/libavfilter.a $OUTPUT_DIR/libavfilter.a
cp -f ffmpeg/libavformat/libavformat.a $OUTPUT_DIR/libavformat.a
cp -f ffmpeg/libavutil/libavutil.a $OUTPUT_DIR/libavutil.a
cp -f ffmpeg/libswresample/libswresample.a $OUTPUT_DIR/libswresample.a
cp -f ffmpeg/libswscale/libswscale.a $OUTPUT_DIR/libswscale.a
