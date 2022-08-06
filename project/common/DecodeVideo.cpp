#include "DecodeVideo.h"

// Emitting video frames is manual, while emitting audio frames is automatic.

int FFmpeg_emit_video_frame(FFmpegContext *context, char* buffer_ptr, int buffer_size) {
    // context->videoFrame contains the latest video frame.

    // Convert the video frame from YUV (or whatever it is) to an RGB frame.
    int result = FFmpegContext_sws_scale_video_frame(context);

    if (result < 0) {
        // Failed to convert the video frame.
        printf("[extension-ffmpeg] Failed to convert video frame.\n");
        return result;
    }

    // Write the newly converted video frame to the output buffer.
    int data_length = av_image_get_buffer_size(FFMPEG_PIXEL_FORMAT, context->videoFrameRGB->width, context->videoFrameRGB->height, 32);
    result = copyVideoFrame(context->videoFrameRGB, buffer_ptr, buffer_size);
    if (result < 0) {
        // Failed to copy the video frame.
        printf("[extension-ffmpeg] Failed to copy video frame.\n");
        return result;
    }

    return 0;
}

int copyVideoFrame(AVFrame* videoFrame, char* buffer_ptr, int buffer_size) {
    // Perform a raw memory copy to move the frame into the pointer.
    char *current_buffer_ptr = buffer_ptr;
	for (int y = 0; y < videoFrame->height; y++)
	{
		int copyCount = videoFrame->width * 4; // 4 bytes per pixel
        if (current_buffer_ptr - buffer_ptr + copyCount > buffer_size)
        {
            // The buffer is too small to hold the frame.
            printf("[extension-ffmpeg] Buffer is too small to hold the video frame.\n");
            return -1;
        }
		memcpy(current_buffer_ptr, videoFrame->data[0]+ y * videoFrame->linesize[0], copyCount);
		current_buffer_ptr += copyCount;
	}
    return 0;
}

/**
 * @brief Decodes one frame from the video stream.
 *
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the frame was decoded successfully.
 */
value __hx_ffmpeg_emit_video_frame(value context, value buffer_value)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);

	buffer data_buffer = val_to_buffer(buffer_value);
	char *data_buffer_ptr = buffer_data(data_buffer);
	int data_buffer_len = buffer_size(data_buffer);

    int result = FFmpeg_emit_video_frame(contextPointer, data_buffer_ptr, data_buffer_len);

    return alloc_int(result);
}

DEFINE_FUNC_2(hx_ffmpeg_emit_video_frame, context, buffer_value)
{
    return __hx_ffmpeg_emit_video_frame(context, buffer_value);
}