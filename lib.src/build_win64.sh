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
FLAGS="$FLAGS --enable-openssl"
# FLAGS="$FLAGS --enable-libvpx"

# Add x64-specific flags here.
FLAGS="$FLAGS --toolchain=msvc"
FLAGS="$FLAGS --enable-static"

pushd ffmpeg
./configure $FLAGS && make
popd

# Export the necessary binaries.
mkdir -p ../lib.build/Windows64/
cp -f ffmpeg/libavcodec/libavcodec.a ../lib.build/Windows64/avcodec.lib
cp -f ffmpeg/libavformat/libavformat.a ../lib.build/Windows64/avformat.lib
cp -f ffmpeg/libavutil/libavutil.a ../lib.build/Windows64/avutil.lib
cp -f ffmpeg/libavdevice/libavdevice.a ../lib.build/Windows64/avdevice.lib
cp -f ffmpeg/libavfilter/libavfilter.a ../lib.build/Windows64/avfilter.lib
cp -f ffmpeg/libswresample/libswresample.a ../lib.build/Windows64/swresample.lib
cp -f ffmpeg/libswscale/libswscale.a ../lib.build/Windows64/swscale.lib