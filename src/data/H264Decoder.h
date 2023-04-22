//
// Created by rolando on 5/15/17.
//

#ifndef DRC_SIM_C_H264DECODER_H
#define DRC_SIM_C_H264DECODER_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
#if LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(54, 6, 0)
    #include <libavutil/imgutils.h>
#endif
};


class H264Decoder {

public:
    H264Decoder();

    uint8_t *decode(uint8_t *nals, int nals_size);

private:
    AVPacket av_packet;
    SwsContext *sws_context;
    AVCodecContext *context;
    AVFrame *frame;
    AVFrame *out_frame;
    uint8_t *out_buffer;
    static void log_av(void * avcl, int level, const char * fmt, va_list vl);
};


#endif //DRC_SIM_C_H264DECODER_H
