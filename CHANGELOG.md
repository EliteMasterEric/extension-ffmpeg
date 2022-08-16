# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<<<<<<< Updated upstream
## [0.0.3] - 2022-08-??
This version seeks to add timing syncronization to the video thread
## Changed
- Locked video frame emission to the presentation timestamp to maintain FPS.

## [0.0.2] - 2022-08-10
This version reworks the decoding pipeline to be multi-threaded, an essential step to syncronization.
## Changed
- Replaced the basic frame output functions with a thread and callback system.
## Known Issues
- Major lag issues when playing audio and video simultaneously.
- When playing video only, frame rate is sometimes too fast (ranging from 70-300 FPS at points, rather than 24)

## [0.0.1] - 2022-08-06
Alpha version with first iteration of video playback.
## Added
- Basic video playback with audio
## Known Issues
- Video and audio desyncronization, audio skipping due to slow decoding
=======
>>>>>>> Stashed changes

## [0.0.2] - 2022-??-??
This version adds the ability to load a file (on the local filesystem or the internet) and display information about it.
## Added
- Added basic version utility functions for debug purposes.
  - `ffmpeg.Version.getAvcodecConfiguration()` retrieves the build flags used for FFmpeg.
  - `ffmpeg.Version.getAvcodecLicense()` retrieves the license for `avcodec`
  - `ffmpeg.Version.getLibavcodecVersion()` retrieves the library version for `avcodec`
  - `ffmpeg.Version.getLibavfilterVersion()` retrieves the library version for `avfilter`
  - `ffmpeg.Version.getLibavformatVersion()` retrieves the library version for `avformat`
  - `ffmpeg.Version.getLibavutilVersion()` retrieves the library version for `avutil`
  - `ffmpeg.Version.getLibswresampleVersion()` retrieves the library version for `swresample`
  - `ffmpeg.Version.getLibswscaleVersion()` retrieves the library version for `swscale`


## [0.0.1] - 2022-07-31
This version establishes the baseline for the project. It ensures the build system is working and that the extension is ready for development.
## Added
- Added basic version utility functions for debug purposes.
  - These functions print information about library versions and the build flags used for FFmpeg.
