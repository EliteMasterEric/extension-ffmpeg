# Compiles FFmpeg for Android using Linux and the androidndk andp3 llvm

# Here we set the android ndk llvm toolchain path.
# You need to choose the folder with your host arch
# And copy the path and paste it inside NDKDIR
NDKDIR="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64"

API="21"
NDK_RANLIB="$NDKDIR/bin/llvm-ranlib"
NDK_CC="$NDKDIR/bin/armv7a-linux-androideabi$API-clang"
NDK_CXX="$NDKDIR/bin/armv7a-linux-androideabi$API-clang++"
NDK_STRIP="$NDKDIR/bin/llvm-strip"
NDK_NM="$NDKDIR/bin/llvm-nm"
NDK_SYSROOT="$NDKDIR/sysroot"

# No this isnt a mistake lol this was done because 
# llvm-as froze configure for some reason
NDK_AS="$NDKDIR/bin/armv7a-linux-androideabi$API-clang"

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
FLAGS="$FLAGS --arch=armeabi-v7a"
FLAGS="$FLAGS --cross-prefix=arm-linux-android-"
FLAGS="$FLAGS --as="$NDK_AS""
FLAGS="$FLAGS --cc="$NDK_CC""
FLAGS="$FLAGS --cxx"$NDK_CX""
FLAGS="$FLAGS --enable-neon" #disable if issues are found
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
OUTPUT_DIR=../lib.build/android-v7
mkdir -p $OUTPUT_DIR
cp -f ffmpeg/libavcodec/libavcodec.a $OUTPUT_DIR/libavcodec.a
cp -f ffmpeg/libavdevice/libavdevice.a $OUTPUT_DIR/libavdevice.a
cp -f ffmpeg/libavfilter/libavfilter.a $OUTPUT_DIR/libavfilter.a
cp -f ffmpeg/libavformat/libavformat.a $OUTPUT_DIR/libavformat.a
cp -f ffmpeg/libavutil/libavutil.a $OUTPUT_DIR/libavutil.a
cp -f ffmpeg/libswresample/libswresample.a $OUTPUT_DIR/libswresample.a
cp -f ffmpeg/libswscale/libswscale.a $OUTPUT_DIR/libswscale.a
