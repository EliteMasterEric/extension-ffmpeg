# extension-ffmpeg

A Haxe library which provides support for handling video, audio, and multimedia files and streams on native platforms.

`extension-ffmpeg` boasts the following features:

- Decoding, display, and playback control for video and audio.
- Support for the most popular video formats, including `avi`, `mp4`, `webm`, `flv`, `mkv`, and more.
- Utilities for easy integration into OpenFL and HaxeFlixel.

Contributions are highly welcome! Feel free to open an issue or pull request if you have any questions or suggestions.

## Building on Windows

To build `extension-ffmpeg` from source, perform these steps to fetch dependencies:
1. Install Visual Studio 2019, installing the following components:
- `MSVC v142 - VS 2019 C++ x64/x86 build tools`
- `Windows SDK (10.0.17763.0)`
2. Install MSYS2.
3. Navigate to the project folder and call `git submodule update --recursive` to download the source for FFmpeg.

Then, follow these steps:
1. Open `x64 Native Tools Command Prompt for VS 2019` and call `"C:\CPP\msys64\msys2_shell.cmd" -use-full-path`.
2. Call `pacman -Su git wget make tar unzip zip mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-cmake autoconf automake libtool mingw-w64-x86_64-jq python zstd nasm` (one-time step).
3. Navigate to the project folder and run `./lib.src/build_winmsys.sh`.

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg windows`.

# Building on Linux

To build `extension-ffmpeg` from source, follow these steps:
1. Install these development packages:
    `libvdpau-dev libva-dev libx11-dev`
2. Navigate to the project folder.
3. Call `git submodule update --recursive` to download the source for FFmpeg.
4. Navigate to `./lib.src/`.
5. Run `./build_linux64.sh`.

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg linux`.

# Building on MacOS

To build `extension-ffmpeg` from source, follow these steps:
1. Navigate to the project folder.
2. Call `git submodule update --recursive` to download the source for FFmpeg.
3. Navigate to `./lib.src/`.
4. Run `./build_linux64.sh`.

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg mac`.

## Building for android

To build `extension-ffmpeg` from source, follow these steps:
1. Download and unzip the android ndk (r23c)
2. Navigate to the project folder.
3. Call `git submodule update --recursive` to download the source for FFmpeg.
4. Navigate to `./lib.src/`.
5. Change the ndk path in `build_android64` to your ndk path,
5. Run `./buikd_android64.sh`.

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg mac`.

this is still on testing

## Building on Other Platforms

Support for other platforms (MacOS, iOS, HTML5 via emscripten) is in development.

## License

The code within the repository of `extension-ffmpeg` is made available under the permissive MIT License.

This software uses code of [FFmpeg](http://ffmpeg.org) licensed under the [LGPLv3.0](https://www.gnu.org/licenses/lgpl-3.0.html) and its sources can be downloaded [here](https://git.ffmpeg.org/ffmpeg).
