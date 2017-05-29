//
// Created by rolando on 5/14/17.
//

#ifndef DRC_SIM_C_VIDEO_WII_U_H
#define DRC_SIM_C_VIDEO_WII_U_H
#include "../../Packet.h"

typedef struct {
    unsigned magic : 4;
    unsigned packet_type : 2;
    unsigned seq_id : 10;
    bool init : 1;
    bool frame_begin : 1;
    bool chunk_end : 1;
    bool frame_end : 1;
    bool has_timestamp : 1;
    unsigned payload_size : 11;
    unsigned timestamp : 32;
    unsigned char extended_header[8]; // Extended header
    uint8_t payload[2048];
} VideoPacketHeaderWiiU;

class VideoPacketWiiU : Packet {

public:
    VideoPacketWiiU(unsigned char *packet, size_t packet_size);

    VideoPacketHeaderWiiU* header;

private:
    void print_debug(unsigned char *packet, size_t packet_size);
};


#endif //DRC_SIM_C_VIDEO_WII_U_H
