//
// Created by rolando on 5/15/17.
//

#include <stdint-gcc.h>
#include "H264Decoder.h"
#include <assert.h>
#include <cstring>
#include "Constants.h"
#include "../util/logging/Logger.h"

H264Decoder::H264Decoder() {
    av_log_set_callback(log_av);
    avcodec_register_all();

    av_init_packet(&av_packet);

    AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    assert(codec != NULL);

    context = avcodec_alloc_context3(codec);
    assert(context != NULL);

    assert(avcodec_open2(context, codec, NULL) == 0);

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55, 28, 1)
    frame = avcodec_alloc_frame();
    out_frame = avcodec_alloc_frame();
#else
    frame = av_frame_alloc();
    out_frame = av_frame_alloc();
#endif
    assert(frame != NULL);
    assert(out_frame != NULL);

    // Dimensions
    sws_context = sws_getContext(WII_VIDEO_WIDTH, WII_VIDEO_HEIGHT, AV_PIX_FMT_YUV420P,
                                 WII_VIDEO_WIDTH, WII_VIDEO_HEIGHT, AV_PIX_FMT_RGB24,
                                 SWS_FAST_BILINEAR, NULL, NULL, NULL);
    int bytes_req;
#if LIBAVUTIL_VERSION_INT < AV_VERSION_INT(54, 6, 0)
    bytes_req = avpicture_get_size(AV_PIX_FMT_RGB24, WII_VIDEO_WIDTH, WII_VIDEO_HEIGHT);
#else
    bytes_req = av_image_get_buffer_size(AV_PIX_FMT_RGB24, WII_VIDEO_WIDTH, WII_VIDEO_HEIGHT, 1);
#endif
    out_buffer = new uint8_t[bytes_req];
#if LIBAVUTIL_VERSION_INT < AV_VERSION_INT(54, 6, 0)
    assert(avpicture_fill((AVPicture *) out_frame, out_buffer, AV_PIX_FMT_RGB24, WII_VIDEO_WIDTH, WII_VIDEO_HEIGHT)
           == bytes_req);
#else
    assert(av_image_fill_arrays(out_frame->data, out_frame->linesize, out_buffer, AV_PIX_FMT_RGB24, WII_VIDEO_WIDTH,
                         WII_VIDEO_HEIGHT, 1) == bytes_req);
#endif
}

size_t H264Decoder::image(uint8_t *nals, int nals_size, uint8_t *image) {
    av_packet.data = nals;
    av_packet.size = nals_size;

    int got_frame = 0;
    int frame_size = avcodec_decode_video2(context, frame, &got_frame, &av_packet);

    if (got_frame) {
        assert(frame_size == av_packet.size);
        sws_scale(sws_context, (const uint8_t *const *) frame->data, frame->linesize, 0, WII_VIDEO_HEIGHT,
                  out_frame->data, out_frame->linesize);
    }
    else
        return 0;
    size_t image_size = out_frame->linesize[0] * WII_VIDEO_HEIGHT;
    memcpy(image, out_frame->data[0], image_size);
    return image_size;
}

void H264Decoder::log_av(void *avcl, int level, const char *fmt, va_list vl) {
    Logger::log("h264", Logger::VERBOSE, fmt, vl);
}
