# Compiles FFmpeg for Windows 64-bit using MSYS2
# https://trac.ffmpeg.org/wiki/CompilationGuide/MinGW

FLAGS=""

# Disable the features we don't want or use to reduce compile times.
FLAGS="$FLAGS --disable-doc"
FLAGS="$FLAGS --disable-programs"

# Direct build logs to a file.
FLAGS="$FLAGS --logfile=../build.log"
# Upgrade license to LGPLv3, allowing use of some ApacheV2 licensed dependencies.
FLAGS="$FLAGS --enable-version3"

# Add HTTPS support.
#FLAGS="$FLAGS --enable-openssl"
#FLAGS="$FLAGS --enable-libvpx"

# Add x86-specific flags here.
flags="$flags --arch=x86"
FLAGS="$FLAGS --toolchain=msvc"
FLAGS="$FLAGS --enable-static"

pushd ffmpeg
./configure $FLAGS && make
popd

# Export the necessary binaries.
mkdir -p ../lib.build/Windows32/
cp -f ffmpeg/libavcodec/libavcodec.a ../lib.build/Windows32/avcodec.lib
cp -f ffmpeg/libavformat/libavformat.a ../lib.build/Windows32/avformat.lib
cp -f ffmpeg/libavutil/libavutil.a ../lib.build/Windows32/avutil.lib
cp -f ffmpeg/libavdevice/libavdevice.a ../lib.build/Windows32/avdevice.lib
cp -f ffmpeg/libavfilter/libavfilter.a ../lib.build/Windows32/avfilter.lib
cp -f ffmpeg/libswresample/libswresample.a ../lib.build/Windows32/swresample.lib
cp -f ffmpeg/libswscale/libswscale.a ../lib.build/Windows32/swscale.lib