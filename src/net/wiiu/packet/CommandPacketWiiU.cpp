//
// Created by rolando on 5/25/17.
//

#include <netinet/in.h>
#include "CommandPacketWiiU.h"
#include "../../../util/logging/Logger.h"

CommandPacketWiiU::CommandPacketWiiU(unsigned char *packet, size_t packet_size) : Packet(packet, packet_size) {
    packet_raw = packet;
    header = (CommandPacketHeaderWiiU*)packet;
#if __BYTE_ORDER == BIG_ENDIAN
    // TODO little to big
#endif
    switch (header->cmd_id) {
        case 0:
            header_cmd0 = (CommandPacketHeaderWiiUCommandZero*)header->payload;
            header_cmd0->error_code = ntohs(header_cmd0->error_code);
            header_cmd0->payload_size = ntohs(header_cmd0->payload_size);
            break;
        case 1:
            header_cmd1 = (CommandPacketHeaderWiiUCommandOne*)header->payload;
            header_cmd1->unknown_0 = ntohs(header_cmd1->unknown_0);
            header_cmd1->mic_volume = ntohs(header_cmd1->mic_volume);
            header_cmd1->mic_volume_2 = ntohs(header_cmd1->mic_volume_2);
            header_cmd1->mic_frequency = ntohs(header_cmd1->mic_frequency);
            header_cmd1->cam_expo_abs = ntohl(header_cmd1->cam_expo_abs);
            header_cmd1->cam_brightness = ntohs(header_cmd1->cam_brightness);
            header_cmd1->cam_contrast = ntohs(header_cmd1->cam_contrast);
            header_cmd1->cam_gain = ntohs(header_cmd1->cam_gain);
            header_cmd1->cam_hue = ntohs(header_cmd1->cam_hue);
            header_cmd1->cam_saturation = ntohs(header_cmd1->cam_saturation);
            header_cmd1->cam_sharpness = ntohs(header_cmd1->cam_sharpness);
            header_cmd1->cam_gamma = ntohs(header_cmd1->cam_gamma);
            header_cmd1->cam_white_balance = ntohl(header_cmd1->cam_white_balance);
            header_cmd1->cam_multiplier = ntohs(header_cmd1->cam_multiplier);
            header_cmd1->cam_multiplier_limit = ntohs(header_cmd1->cam_multiplier_limit);
            break;
        case 2:
            header_cmd2 = (CommandPacketHeaderWiiUCommandTwo*)header->payload;
#if __BYTE_ORDER == BIG_ENDIAN
            // TODO little to big
#endif
            break;
        default:
            Logger::debug(Logger::DRC, "Received unknown command packet type \"%d\" from Wii U.", header->cmd_id);
            break;
    }
}
