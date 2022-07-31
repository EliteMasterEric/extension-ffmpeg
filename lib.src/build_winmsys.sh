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

# Enable to generate dll+lib
#FLAGS="$FLAGS --enable-shared"

# Enable to generate lib only
FLAGS="$FLAGS --enable-static"

# Audio decoding
# FLAGS="$FLAGS --enable-decoder=aac"
# Video decoding
# FLAGS="$FLAGS --enable-decoder=h264"

# DO NOT ENABLE THESE to maintain LGPL compliance
#FLAGS="$FLAGS --enable-gpl"
#FLAGS="$FLAGS --enable-nonfree"

pushd ffmpeg
./configure $FLAGS && make
popd

# Export the necessary binaries.
mkdir -p ../lib.build/Windows/
#cp -f ffmpeg/libavcodec/avcodec.lib ../lib.build/Windows/avcodec.lib
#cp -f ffmpeg/libavformat/avformat.lib ../lib.build/Windows/avformat.lib
#cp -f ffmpeg/libavutil/avutil.lib ../lib.build/Windows/avutil.lib
#cp -f ffmpeg/libavcodec/libavcodec.a ../lib.build/Windows/libavcodec.a
#cp -f ffmpeg/libavformat/libavformat.a ../lib.build/Windows/libavformat.a
#cp -f ffmpeg/libavutil/libavutil.a ../lib.build/Windows/libavutil.a
#cp -f ffmpeg/libavdevice/libavdevice.a ../lib.build/Windows/libavdevice.a
#cp -f ffmpeg/libavfilter/libavfilter.a ../lib.build/Windows/libavfilter.a
#cp -f ffmpeg/libswresample/libswresample.a ../lib.build/Windows/libswresample.a
#cp -f ffmpeg/libswscale/libswscale.a ../lib.build/Windows/libswscale.a
