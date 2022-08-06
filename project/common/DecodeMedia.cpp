#include "DecodeVideo.h"

int FFmpeg_decodeFrame(FFmpegContext *context, value emitAudioCallback)
{
    AVPacket pkt;

    // Read from the context into the packet.
    int result = av_read_frame(context->avFormatCtx, &pkt);
    if (result < 0)
    {
        // Failed to read from the context.
        printf("[extension-ffmpeg] Failed to read next packet.\n");
        return result;
    }

    if (pkt.stream_index == context->videoStreamIndex)
    {
        printf("Packet contains a video frame.\n");

        if (context->videoCodecCtx == nullptr)
        {
            printf("[extension-ffmpeg] Video codec context not initialized.\n");
            result = 0;
        }
        else
        {
            // Send the data packet to the decoder.
            result = avcodec_send_packet(context->videoCodecCtx, &pkt);
            if (result < 0)
            {
                // Failed to send the packet to the decoder.
                printf("[extension-ffmpeg] Failed to send packet to video decoder.\n");
            }
            else
            {
                // Receive the frame back from the decoder.
                result = avcodec_receive_frame(context->videoCodecCtx, context->videoFrame);
                if (result < 0)
                {
                    // Failed to decode the packet.
                    printf("[extension-ffmpeg] Failed to decode video packet.\n");
                }
                else
                {
                    printf("Success.\n");
                    // Positive 1 = successful video frame.
                    result = 1;
                }
            }
        }
    }
    else if (pkt.stream_index == context->audioStreamIndex)
    {
        printf("Packet contains an audio frame\n");

        if (context->audioCodecCtx == nullptr)
        {
            printf("[extension-ffmpeg] Audio codec context not initialized.\n");
            result = 0;
        }
        else
        {
            printf("Pointer: %p\n", context->audioCodecCtx);
            // Send the data packet to the decoder.
            printf("Sending audio packet to decoder...\n");
            result = avcodec_send_packet(context->audioCodecCtx, &pkt);
            if (result < 0)
            {
                // Failed to send the packet to the decoder.
                printf("[extension-ffmpeg] Failed to send packet to audio decoder.\n");
            }
            else
            {
                // Receive the frame back from the decoder.
                printf("Receiving audio frame from decoder...\n");
                result = avcodec_receive_frame(context->audioCodecCtx, context->audioFrame);
                if (result < 0)
                {
                    // Failed to decode the packet.
                    printf("[extension-ffmpeg] Failed to decode audio packet.\n");
                    return result;
                }
                else
                {
                    printf("Emitting audio frame...\n");
                    result = FFmpeg_emit_audio_frame(context, emitAudioCallback);
                    if (result < 0)
                    {
                        // Failed to convert the audio frame.
                        printf("[extension-ffmpeg] Failed to convert audio frame.\n");
                        return result;
                    }

                    // Positive 2 = successful audio frame.
                    result = 2;
                }
            }
        }
    }
    else if (pkt.stream_index == context->subtitleStreamIndex)
    {
        printf("Packet contains a subtitle frame\n");
        result = 0;
    }
    else
    {
        // Unknown stream index for frame.
        printf("Packet contains an unknown frame\n");
        result = 0;
    }

    // Free the packet.
    av_packet_unref(&pkt);

    return result;
}

/**
 * @brief Decodes one frame from the video stream.
 *
 * @param context A Haxe object wrapping the FFmpegContext to populate.
 * @return Whether the frame was decoded successfully.
 */
value __hx_ffmpeg_decode_frame(value context, value emitAudioCallback)
{
    FFmpegContext *contextPointer = FFmpegContext_unwrap(context);
    int result = FFmpeg_decodeFrame(contextPointer, emitAudioCallback);

    return alloc_int(result);
}

DEFINE_FUNC_2(hx_ffmpeg_decode_frame, context, emitAudioCallback)
{
    return __hx_ffmpeg_decode_frame(context, emitAudioCallback);
}