#include "DecodeVideo.h"

int FFmpeg_decodeFrame(FFmpegContext *context, value emit_video, value emit_audio)
{
    // Allocate space for the packet.
    if (context->avPacket == NULL)
    {
        context->avPacket = av_packet_alloc();
    }

    // Read from the context into the packet.
    int result = av_read_frame(context->avFormatCtx, context->avPacket);
    if (result < 0)
    {
        // Failed to read from the context.
        printf("[extension-ffmpeg] Failed to read next packet.\n");
        return result;
    }

    if (context->avPacket->stream_index == context->videoStreamIndex)
    {
        printf("Packet contains a video frame.\n");

        // Send the data packet to the decoder.
        result = avcodec_send_packet(context->videoCodecCtx, context->avPacket);
        if (result < 0)
        {
            // Failed to send the packet to the decoder.
            printf("[extension-ffmpeg] Failed to send packet to decoder.\n");
            return result;
        }


        // Receive the frame back from the decoder.
        result = avcodec_receive_frame(context->videoCodecCtx, context->videoFrame);
        if (result < 0)
        {
            // Failed to decode the packet.
            printf("[extension-ffmpeg] Failed to decode video packet.\n");
            return result;
        }

        // Emit the video frame.
        result = FFmpeg_emit_video_frame(context, emit_video);
        if (result < 0)
        {
            // Failed to emit the video frame.
            printf("[extension-ffmpeg] Failed to emit video frame.\n");
            return result;
        }

        printf("Success.\n");
        // Positive 1 = successful video frame emission.
        return 1;
    }
    else if (context->avPacket->stream_index == context->audioStreamIndex)
    {
        printf("Packet contains an audio frame\n");
        // Decode the packet.
        // result = avcodec_receive_frame(context->audioCodecCtx, context->audioFrame);
        // if (result < 0)
        // {
        //     // Failed to decode the packet.
        //     printf("[extension-ffmpeg] Failed to decode audio packet.\n");
        //     return result;
        // }
    }
    else if (context->avPacket->stream_index == context->subtitleStreamIndex)
    {
        printf("Packet contains a subtitle frame\n");
        // Decode the packet.
        // result = avcodec_receive_frame(context->subtitleCodecCtx, context->subtitleFrame);
        // if (result < 0)
        // {
        //     // Failed to decode the packet.
        //     printf("[extension-ffmpeg] Failed to decode subtitle packet.\n");
        //     return result;
        // }
    }
    else
    {
        // Unknown stream index for frame.
        printf("Packet contains an unknown frame\n");
    }

    return 0;
}

/**
 * @brief Decodes one frame from the video stream.
 *
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the frame was decoded successfully.
 */
value __hx_ffmpeg_decode_frame(value context, value emit_video, value emit_audio)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    int result = FFmpeg_decodeFrame(contextPointer, emit_video, emit_audio);

    return alloc_int(result);
}

DEFINE_FUNC_3(hx_ffmpeg_decode_frame, context, emit_video, emit_audio)
{
    return __hx_ffmpeg_decode_frame(context, emit_video, emit_audio);
}