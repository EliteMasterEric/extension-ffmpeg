# extension-ffmpeg

An OpenFL extension which provides support for handling video, audio, and multimedia files and streams on native platforms.

Focus is currently on video decoding in H264 format, and audio decoding in AAC format. Wider support is planned for the future.

Contributions are highly welcome! Feel free to open an issue or pull request if you have any questions or suggestions.

## Building on Windows

https://trac.ffmpeg.org/wiki/CompilationGuide/CrossCompilingForWindows

This will generate the necessary `.a` files that you need to build the extension.

To build the extension itself, run `lime rebuild extension-ffmpeg windows`.

## License

The code within the repository of `extension-ffmpeg` is made available under the permissive MIT License.

This software uses code of [FFmpeg](http://ffmpeg.org) licensed under the [LGPLv3.0](https://www.gnu.org/licenses/lgpl-3.0.html) and its sources can be downloaded [here](https://git.ffmpeg.org/ffmpeg).
