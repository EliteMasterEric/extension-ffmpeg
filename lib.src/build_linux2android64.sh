# Compiles FFmpeg for Android using Linux and the androidndk (r23c)

# Here we set the android ndk (r23c) llvm toolchain path.
# You need to choose the folder with your host arch
# And copy the path and paste it inside NDKDIR
NDKDIR="~/Android/Sdk/ndk/25.0.8775105/toolchains/llvm/prebuilt/linux-x86_64"

API="21"
NDK_RANLIB="$NDKDIR/bin/llvm-ranlib"
NDK_CC="$NDKDIR/bin/aarch64-linux-android$API-clang"
NDK_CXX="$NDKDIR/bin/aarch64-linux-android$API-clang++"
NDK_STRIP="$NDKDIR/bin/llvm-strip"
NDK_NM="$NDKDIR/bin/llvm-nm"
NDK_SYSROOT="$NDKDIR/sysroot"

# No this isnt a mistake lol this was done because 
# llvm-as froze configure for some reason
NDK_AS="$NDKDIR/bin/aarch64-linux-android$API-clang"

NDK_AR="$NDKDIR/bin/llvm-ar"

# Yep not fancy but it works soo
# Also the reason to why theres no openssl support 
# Its because we would need to compile the entirety of openssl
FLAGS=""

FLAGS="$FLAGS --disable-doc"
FLAGS="$FLAGS --disable-programs"

# Direct build logs to a file.
FLAGS="$FLAGS --logfile=../build.log"
# Upgrade license to LGPLv3, allowing use of some ApacheV2 licensed dependencies.
FLAGS="$FLAGS --enable-version3"

# Android-specific flags.
FLAGS="$FLAGS --enable-cross-compile"
FLAGS="$FLAGS --target-os=android"
FLAGS="$FLAGS --arch=arm64"
FLAGS="$FLAGS --cpu=armv8-a"
FLAGS="$FLAGS --cross-prefix=aarch64-linux-gnu-"
FLAGS="$FLAGS --as="$NDK_AS""
FLAGS="$FLAGS --cc="$NDK_CC""
FLAGS="$FLAGS --cxx"$NDK_CX""
FLAGS="$FLAGS --sysroot="$NDK_SYSROOT""
FLAGS="$FLAGS --strip="$NDK_STRIP""
FLAGS="$FLAGS --cross-prefix="$NDKDIR""
FLAGS="$FLAGS --nm="$NDK_NM""
FLAGS="$FLAGS --ar="$NDK_AR""
FLAGS="$FLAGS --ranlib="$NDK_RANLIB""

echo "FLAGS: $FLAGS"

pushd ffmpeg
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
