//
// Created by rolando on 5/14/17.
//

#include <iostream>
#include <sstream>
#include <assert.h>
#include "VideoPacketWiiU.h"
#include "../../../util/BitUtil.h"
#include "../../../util/logging/Logger.h"

using namespace std;

VideoPacketWiiU::VideoPacketWiiU(unsigned char *packet, size_t packet_size) : Packet(packet, packet_size) {
    // Parse
    header = (VideoPacketHeaderWiiU*)packet;
    if (Logger::get_level() >= Logger::VERBOSE)
        print_debug(packet, packet_size);
#if __BYTE_ORDER == LITTLE_ENDIAN
    for (int byte = 0; byte < packet_size; byte++)
        packet[byte] = (unsigned char) BitUtil::reverse(packet[byte], 8);
    header->magic = BitUtil::reverse(header->magic, 4);
    header->packet_type = BitUtil::reverse(header->packet_type, 2);
    header->seq_id = BitUtil::reverse(header->seq_id, 10);
    header->payload_size = BitUtil::reverse(header->payload_size, 11);
    header->timestamp = BitUtil::reverse(header->timestamp, 32);
    for (int byte = 0; byte < sizeof(header->extended_header); ++byte)
        header->extended_header[byte] = (unsigned char) BitUtil::reverse(header->extended_header[byte], 8);
    for (int byte = 0; byte < header->payload_size; ++byte)
        header->payload[byte] = (unsigned char) BitUtil::reverse(header->payload[byte], 8);
#endif
    assert(header->payload_size <= 2048);
}

void VideoPacketWiiU::print_debug(unsigned char *packet, size_t packet_size) {
    stringstream hex_string;
    hex_string << hex;
    for (int byte = 0; byte < packet_size; byte++)
        hex_string << (int) packet[byte] << " ";
    hex_string << endl << dec;
    hex_string << "  magic = " << header->magic << endl;
    hex_string << "  packet_type = " << header->packet_type << endl;
    hex_string << "  seq_id = " << header->seq_id << endl;
    hex_string << "  init = " << header->init << endl;
    hex_string << "  frame_begin = " << header->frame_begin << endl;
    hex_string << "  chunk_end = " << header->chunk_end << endl;
    hex_string << "  frame_end = " << header->frame_end << endl;
    hex_string << "  has_timestamp = " << header->has_timestamp << endl;
    hex_string << "  payload_size = " << header->payload_size << endl;
    hex_string << "  timestamp = " << header->timestamp;
    Logger::verbose(Logger::DRC, "Video packet:\n  %s", hex_string.str().c_str());
}
