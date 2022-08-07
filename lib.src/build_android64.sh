# Compiles FFmpeg for Android using MSYS2 and the androidndk (r23c)

# Here we set the android ndk (r23c) llvm toolchain path.
# You need to choose the folder with your host arch and copy the path and paste it inside there
NDKDIR="C:/your/ndk/path/here"

RANLIB="$NDKDIR/bin/llvm-ranlib"
CC="$NDKDIR/aarch64-linux-android21-clang"
CXX="$NDKDIR/aarch64-linux-android21-clang++"
STRIP="$NDKDIR/llvm-strip"
NM="$NDKDIR/bin/llvm-nm"
AS="$NDKDIR/bin/aarch64-linux-android21-clang"
AR="$NDKDIR/bin/llvm-ar"

# Yep not fancy but it works soo
FLAGS="--disable-doc --disable-programs --enable-version3 --arch="aarch64" --target-os=android --as="$AS" --cc="$CC" --cxx="$CXX" --strip="$STRIP" --enable-cross-compile  --cross-prefix="$NDKPREFIX" --nm="$NM" --ar="$AR" --ranlib="$RANLIB""

pushd ffmpeg
#make clean &&
./configure $FLAGS && make
popd

# Export the necessary binaries.
OUTPUT_DIR=../lib.build/Android64/
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
