# extension-ffmpeg

An OpenFL extension which provides support for handling video, audio, and multimedia files and streams on native platforms.

Focus is currently on video decoding in H264 format, and audio decoding in AAC format. Wider support is planned for the future.

Contributions are highly welcome! Feel free to open an issue or pull request if you have any questions or suggestions.

## Building on Windows

To build `extension-ffmpeg` from source, install these dependencies:
1. Install Visual Studio 2019, installing the following components:
- `MSVC v142 - VS 2019 C++ x64/x86 build tools`
- `Windows SDK (10.0.17763.0)`
2. Install MSYS2.

Then, follow these steps:
1. Open `x64 Native Tools Command Prompt for VS 2019` and call `"C:\CPP\msys64\msys2_shell.cmd" -use-full-path`.
2. Call `pacman -Su git wget make tar unzip zip mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-cmake autoconf automake libtool mingw-w64-x86_64-jq python zstd nasm` (one-time step).
3. Navigate to the project folder and run `./lib.src/build_winmsys.sh`.

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg windows`.

## License

The code within the repository of `extension-ffmpeg` is made available under the permissive MIT License.

This software uses code of [FFmpeg](http://ffmpeg.org) licensed under the [LGPLv3.0](https://www.gnu.org/licenses/lgpl-3.0.html) and its sources can be downloaded [here](https://git.ffmpeg.org/ffmpeg).
