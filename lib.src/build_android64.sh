# Compiles FFmpeg for Android using MSYS2 and the androidndk (r23c)

# Here we set the android ndk (r23c) llvm toolchain path.
# You need to choose the folder with your host arch
# And copy the path and paste it inside NDKDIR
NDKDIR="/your/ndk/path/here"

# If some of the bin locations look weird to you
# Trust me, it compiles with this
NDK_RANLIB="$NDKDIR/bin/llvm-ranlib"
NDK_CC="$NDKDIR/bin/aarch64-linux-android21-clang"
NDK_CXX="$NDKDIR/bin/aarch64-linux-android21-clang++"
NDK_STRIP="$NDKDIR/bin/llvm-strip"
NDK_NM="$NDKDIR/bin/llvm-nm"

# No this isnt a mistake lol this was done because 
# llvm-as froze configure for some reason
NDK_AS="$NDKDIR/bin/aarch64-linux-android21-clang"

NDK_AR="$NDKDIR/bin/llvm-ar"

# Yep not fancy but it works soo
# Also the reason to why theres no openssl support 
# Its because its kinda complex
FLAGS="--disable-doc --disable-programs --enable-version3 --arch="aarch64" --target-os=android --as="$NDK_AS" --cc="$NDK_CC" --cxx="$NDK_CXX" --strip="$NDK_STRIP" --enable-cross-compile  --cross-prefix="$NDKDIR" --nm="$NDK_NM" --ar="$NDK_AR" --ranlib="$NDK_RANLIB""

# You can also compile from termux with the ndk with this

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
