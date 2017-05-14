//
// Created by rolando on 5/14/17.
//

#ifndef DRC_SIM_C_VIDEO_H
#define DRC_SIM_C_VIDEO_H


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
    unsigned extended_header : 8 * 8; // Extended header
    unsigned payload;
} VideoPacketHeader;

class VideoPacket {

public:
    VideoPacket(unsigned char *packet, size_t packet_size);

    VideoPacketHeader* header;

private:
    void print_debug(unsigned char *packet, size_t packet_size);
};


#endif //DRC_SIM_C_VIDEO_H
