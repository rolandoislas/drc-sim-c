//
// Created by rolando on 5/17/17.
//

#ifndef DRC_SIM_C_COMMANDPACKET_SERVER_H
#define DRC_SIM_C_COMMANDPACKET_SERVER_H

#include <cstdint>
#include "../../Packet.h"

typedef struct {
    uint16_t type;
    uint16_t payload_size;
    unsigned char payload[];
} CommandPacketHeaderServer;

typedef struct {
    uint16_t button;
} CommandPacketButtonServer;

typedef struct {
    uint16_t left_x;
    uint16_t left_y;
    uint16_t right_x;
    uint16_t right_y;
} CommandPacketJoystickServer;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} CommandPacketTouchServer;

typedef struct {
    uint8_t extra;
} CommandPacketButtonExtraServer;

class CommandPacketServer : Packet{
public:
    CommandPacketServer(unsigned char *packet, size_t packet_size);

    CommandPacketHeaderServer * header;
    CommandPacketButtonServer *button;
    CommandPacketJoystickServer *joystick;
    CommandPacketTouchServer *touch;
    CommandPacketButtonExtraServer *button_extra;

    const static uint16_t COMMAND_REGISTER = 0;
    const static uint16_t COMMAND_PING = 1;
    const static uint16_t COMMAND_PONG = 2;
    const static uint16_t COMMAND_INPUT_VIBRATE = 3;
    const static uint16_t COMMAND_INPUT_MIC_BLOW = 4;
    const static uint16_t COMMAND_INPUT_BUTTON = 5;
    const static uint16_t COMMAND_INPUT_JOYSTICK = 6;
    const static uint16_t COMMAND_INPUT_TOUCH = 7;
    const static uint16_t COMMAND_INPUT_BUTTON_EXTRA = 8;
};


#endif //DRC_SIM_C_COMMANDPACKET_SERVER_H
