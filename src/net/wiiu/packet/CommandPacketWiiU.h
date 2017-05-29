//
// Created by rolando on 5/25/17.
//

#ifndef DRC_SIM_C_COMMANDPACKETWIIU_H
#define DRC_SIM_C_COMMANDPACKETWIIU_H


#include <cstdint>
#include "../../Packet.h"

typedef struct {
    // Little endian
    uint16_t packet_type;
    uint16_t cmd_id;
    uint16_t payload_size;
    uint16_t seq_id;
    uint8_t payload[2048];
} CommandPacketHeaderWiiU;

typedef struct {
    // Big endian
    uint8_t magic;
    uint8_t unknown_0;
    uint8_t unknown_1;
    uint8_t unknown_2;
    uint8_t unknown_3;
    uint8_t flags;
    uint8_t id_primary;
    uint8_t id_secondary;
    uint16_t error_code;
    uint16_t payload_size;
    uint8_t payload[2048];
} CommandPacketHeaderWiiUCommandZero;

typedef struct {
    // Big endian
    uint8_t f1;
    uint16_t unknown_0;
    uint8_t f3;
    uint8_t mic_enabled;
    uint8_t mic_mute;
    uint16_t mic_volume;
    uint16_t mic_volume_2;
    uint8_t unknown_a;
    uint8_t unknown_b;
    uint16_t mic_frequency;
    uint8_t cam_enabled;
    uint8_t cam_power;
    uint8_t cam_power_freq;
    uint8_t cam_auto_expo;
    uint32_t cam_expo_abs;
    uint16_t cam_brightness;
    uint16_t cam_contrast;
    uint16_t cam_gain;
    uint16_t cam_hue;
    uint16_t cam_saturation;
    uint16_t cam_sharpness;
    uint16_t cam_gamma;
    uint8_t cam_key_frame;
    uint8_t cam_white_balance_auto;
    uint32_t cam_white_balance;
    uint16_t cam_multiplier;
    uint16_t cam_multiplier_limit;
    uint8_t padding[2];
} CommandPacketHeaderWiiUCommandOne;

typedef struct {
    // Little endian
    uint16_t Jdn_base;
    uint8_t padding[2];
    uint32_t seconds;
} CommandPacketHeaderWiiUCommandTwo;

class CommandPacketWiiU : Packet {
public:
    CommandPacketWiiU(unsigned char *packet, size_t packet_size);

    CommandPacketHeaderWiiU *header;
    CommandPacketHeaderWiiUCommandZero *header_cmd0;
    CommandPacketHeaderWiiUCommandOne *header_cmd1;
    CommandPacketHeaderWiiUCommandTwo *header_cmd2;
    const static uint16_t PT_REQ = 0;
    const static uint16_t PT_REQ_ACK = 1;
    const static uint16_t PT_RESP = 2;
    const static uint16_t PT_RESP_ACK = 3;
    const static uint16_t CMD0_OK = 0;
    unsigned char *packet_raw;
};


#endif //DRC_SIM_C_COMMANDPACKETWIIU_H
