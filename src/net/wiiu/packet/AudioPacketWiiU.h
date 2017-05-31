//
// Created by rolando on 5/26/17.
//

#ifndef DRC_SIM_C_AUDIOPACKETWIIU_H
#define DRC_SIM_C_AUDIOPACKETWIIU_H


#include <cstdint>
#include "../../Packet.h"

#pragma pack(1)
typedef struct {
    unsigned format : 3;
    bool mono : 1;
    bool vibrate : 1;
    unsigned type : 1;
    unsigned seq_id : 10;
    unsigned payload_size : 16;
    unsigned timestamp : 32;
    unsigned char payload[2048];
} AudioPacketHeaderWiiU;
#pragma pack(0)

typedef struct {
    uint32_t timestamp;
    uint32_t unknown_freq_0[2];
    uint32_t unknown_freq_1[2];
    uint32_t video_format;
} AudioPacketVideoFormatWiiU;

class AudioPacketWiiU : Packet {
public:
    AudioPacketWiiU(unsigned char *packet, size_t packet_size);

    AudioPacketHeaderWiiU *header;
    const static unsigned int TYPE_AUDIO = 0;
    const static unsigned int TYPE_VIDEO = 1;
    AudioPacketVideoFormatWiiU *header_video_format;
};


#endif //DRC_SIM_C_AUDIOPACKETWIIU_H
