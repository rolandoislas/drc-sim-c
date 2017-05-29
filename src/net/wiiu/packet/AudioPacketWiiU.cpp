//
// Created by rolando on 5/26/17.
//

#include <netinet/in.h>
#include "AudioPacketWiiU.h"
#include "../../../util/BitUtil.h"

AudioPacketWiiU::AudioPacketWiiU(unsigned char *packet, size_t packet_size) : Packet(packet, packet_size) {
    header = (AudioPacketHeaderWiiU *)packet;
#if __BYTE_ORDER == LITTLE_ENDIAN
    for (int byte = 0; byte < packet_size; byte++)
        packet[byte] = (unsigned char) BitUtil::reverse(packet[byte], 8);
    header->format = BitUtil::reverse(header->format, 3);
    header->seq_id = BitUtil::reverse(header->seq_id, 10);
    header->payload_size = BitUtil::reverse(header->payload_size, 16);
    header->timestamp = BitUtil::reverse(header->timestamp, 32);
    for (int byte = 0; byte < header->payload_size; ++byte)
        header->payload[byte] = (unsigned char) BitUtil::reverse(header->payload[byte], 8);
#endif
    if (header->type == TYPE_VIDEO) {
        header_video_format = (AudioPacketVideoFormatWiiU *) header->payload;
        header_video_format->timestamp = ntohl(header_video_format->timestamp);
        header_video_format->video_format = ntohl(header_video_format->video_format);
    }
}
