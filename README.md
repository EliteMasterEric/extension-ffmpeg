# extension-ffmpeg

A Haxe library which provides support for handling video, audio, and multimedia files and streams on native platforms.

`extension-ffmpeg` boasts the following features:

- Decoding, display, and playback control for video and audio.
- Support for the most popular video formats, including `avi`, `mp4`, `webm`, `flv`, `mkv`, and more.
- Utilities for easy integration into OpenFL and HaxeFlixel.

Contributions are highly welcome! Feel free to open an issue or pull request if you have any questions or suggestions.

## Building on Windows

To build `extension-ffmpeg` from source, perform these steps to fetch dependencies:
1. Do NOT download the repository by clicking the `Download ZIP` button on the Github page. 
- Instead, run `git clone https://github.com/MasterEric/extension-ffmpeg` to create a local Git repository.
2. Install Visual Studio 2019, installing the following components:
- `MSVC v142 - VS 2019 C++ x64/x86 build tools`
- `Windows SDK (10.0.17763.0)`
3. Install MSYS2.
4. Navigate to the project folder and call `git submodule update --init --recursive --remote` to download the source for FFmpeg.

Then, follow these steps:
1. Open `x64 Native Tools Command Prompt for VS 2019` and call `"C:\CPP\msys64\msys2_shell.cmd" -use-full-path`.
2. Call `pacman -Su git wget make tar unzip zip mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-cmake autoconf automake libtool mingw-w64-x86_64-jq python zstd nasm` (one-time step).
3. Navigate to the project folder and run `./lib.src/build_winmsys.sh`.

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg windows`. You will need to repeat this step if you change any of the C++ files located in the `project` folder.

# Building on Linux

To build `extension-ffmpeg` from source, follow these steps:
1. Do NOT download the repository by clicking the `Download ZIP` button on the Github page. 
- Instead, run `git clone https://github.com/MasterEric/extension-ffmpeg` to create a local Git repository.
2. Install these development packages:
    `libvdpau-dev libva-dev libx11-dev`
3. Navigate to the project folder.
4. Call `git submodule update --init --recursive --remote` to download the source for FFmpeg.
5. Navigate to `./lib.src/`.
6. Run `./build_linux64.sh`.
- You will need to repeat this step if you modify the compile flags in the script (which are used to build FFmpeg's libs).

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg linux`. You will need to repeat this step if you change any of the C++ files located in the `project` folder.

# Building on MacOS

To build `extension-ffmpeg` from source, follow these steps:
1. Do NOT download the repository by clicking the `Download ZIP` button on the Github page. 
- Instead, run `git clone https://github.com/MasterEric/extension-ffmpeg` to create a local Git repository.
2. Use [Brew](https://brew.sh/) to install dependencies:
- `brew install nasm libressl`
3. Navigate to the project folder.
4. Call `git submodule update --init --recursive --remote` to download the source for FFmpeg.
5. Navigate to `./lib.src/`.
6. Run `./build_mac64.sh`.
- You will need to repeat this step if you modify the compile flags in the script (which are used to build FFmpeg's libs).

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg mac`. You will need to repeat this step if you change any of the C++ files located in the `project` folder.

## Building on Other Platforms

Support for other platforms (MacOS, Android, iOS, HTML5 via emscripten) is in development.

## License

The code within the repository of `extension-ffmpeg` is made available under the permissive MIT License.

This software uses code of [FFmpeg](http://ffmpeg.org) licensed under the [LGPLv3.0](https://www.gnu.org/licenses/lgpl-3.0.html) and its sources can be downloaded [here](https://git.ffmpeg.org/ffmpeg).
