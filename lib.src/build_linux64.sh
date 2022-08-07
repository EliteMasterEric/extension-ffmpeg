# Compiles FFmpeg for 64-bit Linux

# Compilation flags for FFmpeg
FLAGS=""

# Cross-compilation flags.
FLAGS="$FLAGS --arch=x64"

# Disable the features we don't want or use to reduce compile times.
FLAGS="$FLAGS --disable-doc"
FLAGS="$FLAGS --disable-programs"

# Direct build logs to a file.
FLAGS="$FLAGS --logfile=../build.log"
# Upgrade license to LGPLv3, allowing use of some ApacheV2 licensed dependencies.
FLAGS="$FLAGS --enable-version3"

# Add HTTPS support.
FLAGS="$FLAGS --enable-openssl"

pushd ffmpeg
make clean && ./configure $FLAGS && make
popd

# Export the necessary binaries.
OUTPUT_DIR=../lib.build/Linux64/
mkdir -p $OUTPUT_DIR
cp -f ffmpeg/libavcodec/libavcodec.a $OUTPUT_DIR/libavcodec.a
cp -f ffmpeg/libavdevice/libavdevice.a $OUTPUT_DIR/libavdevice.a
cp -f ffmpeg/libavfilter/libavfilter.a $OUTPUT_DIR/libavfilter.a
cp -f ffmpeg/libavformat/libavformat.a $OUTPUT_DIR/libavformat.a
cp -f ffmpeg/libavutil/libavutil.a $OUTPUT_DIR/libavutil.a
cp -f ffmpeg/libswresample/libswresample.a $OUTPUT_DIR/libswresample.a
cp -f ffmpeg/libswscale/libswscale.a $OUTPUT_DIR/libswscale.a
