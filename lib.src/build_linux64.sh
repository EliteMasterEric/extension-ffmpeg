# Compiles FFmpeg for Windows using MSYS2

# Compilation flags for FFmpeg
FLAGS=""

# Cross-compilation flags.
#FLAGS="$FLAGS --host=i686-linux-gnu \"CFLAGS=-m32\" \"CXXFLAGS=-m32\" \"LDFLAGS=-m32\""
FLAGS="$FLAGS --arch=x64"

# Here we disable all FFmpeg's features. We then enable only the features we need.
# This means we don't have to include encoders, and formats we aren't going to use.
#FLAGS="$FLAGS --disable-everything"
FLAGS="$FLAGS --disable-doc"
FLAGS="$FLAGS --disable-programs"

# Direct build logs to a file.
FLAGS="$FLAGS --logfile=../build.log"
# Upgrade license to LGPLv3, allowing use of some ApacheV2 licensed dependencies.
FLAGS="$FLAGS --enable-version3"

pushd ffmpeg
#make clean &&
./configure $FLAGS && make
popd

# Export the necessary binaries.
OUTPUT_DIR=../lib.build/Linux64/
mkdir -p $OUTPUT_DIR
cp -f ffmpeg/libavcodec/libavcodec.so $OUTPUT_DIR/libavcodec.so
cp -f ffmpeg/libavdevice/libavdevice.so $OUTPUT_DIR/libavdevice.so
cp -f ffmpeg/libavfilter/libavfilter.so $OUTPUT_DIR/libavfilter.so
cp -f ffmpeg/libavformat/libavformat.so $OUTPUT_DIR/libavformat.so
cp -f ffmpeg/libavutil/libavutil.so $OUTPUT_DIR/libavutil.so
cp -f ffmpeg/libswresample/libswresample.so $OUTPUT_DIR/libswresample.so
cp -f ffmpeg/libswscale/libswscale.so $OUTPUT_DIR/libswscale.so
cp -f ffmpeg/libavcodec/libavcodec.a $OUTPUT_DIR/libavcodec.a
cp -f ffmpeg/libavdevice/libavdevice.a $OUTPUT_DIR/libavdevice.a
cp -f ffmpeg/libavfilter/libavfilter.a $OUTPUT_DIR/libavfilter.a
cp -f ffmpeg/libavformat/libavformat.a $OUTPUT_DIR/libavformat.a
cp -f ffmpeg/libavutil/libavutil.a $OUTPUT_DIR/libavutil.a
cp -f ffmpeg/libswresample/libswresample.a $OUTPUT_DIR/libswresample.a
cp -f ffmpeg/libswscale/libswscale.a $OUTPUT_DIR/libswscale.a