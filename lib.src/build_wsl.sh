# Compiles FFmpeg for Windows using MSYS2

# Compilation flags for FFmpeg
FLAGS=""

FLAGS="$FLAGS --toolchain=msvc"

# Here we disable all FFmpeg's features. We then enable only the features we need.
# This means we don't have to include encoders, and formats we aren't going to use.
FLAGS="$FLAGS --disable-everything"
FLAGS="$FLAGS --disable-doc"
FLAGS="$FLAGS --disable-programs"

# Direct build logs to a file.
FLAGS="$FLAGS --logfile=../build.log"
# Upgrade license to LGPLv3, allowing use of some ApacheV2 licensed dependencies.
FLAGS="$FLAGS --enable-version3"

# Audio decoding
# FLAGS="$FLAGS --enable-decoder=aac"
# Video decoding
# FLAGS="$FLAGS --enable-decoder=h264"

# Cross-compilation flags
FLAGS="$FLAGS --arch=x86"
FLAGS="$FLAGS --target-os=mingw32 --cross-prefix=i686-w64-mingw32-"

#FLAGS="$FLAGS --enable-shared"

# DO NOT ENABLE THESE to maintain LGPL compliance
#FLAGS="$FLAGS --enable-gpl"
#FLAGS="$FLAGS --enable-nonfree"

pushd ffmpeg
./configure $FLAGS && make
popd

# Export the necessary binaries.
mkdir -p ../lib.build/Windows64/
cp -f ffmpeg/libavutil/libavutil.a ../lib.build/Windows64/libavutil.a
cp -f ffmpeg/libavcodec/libavcodec.a ../lib.build/Windows64/libavcodec.a
#cp -f ffmpeg/libavdevice/libavdevice.a ../lib.build/Windows64/libavdevice.a
#cp -f ffmpeg/libavfilter/libavfilter.a ../lib.build/Windows64/libavfilter.a
#cp -f ffmpeg/libavformat/libavformat.a ../lib.build/Windows64/libavformat.a
#cp -f ffmpeg/libswresample/libswresample.a ../lib.build/Windows64/libswresample.a
#cp -f ffmpeg/libswscale/libswscale.a ../lib.build/Windows64/libswscale.a
