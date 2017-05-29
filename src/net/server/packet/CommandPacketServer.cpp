//
// Created by rolando on 5/17/17.
//

#include <netinet/in.h>
#include "CommandPacketServer.h"

CommandPacketServer::CommandPacketServer(unsigned char *packet, size_t packet_size) : Packet(packet, packet_size) {
    header = (CommandPacketHeaderServer*) packet;
    header->type = ntohs(header->type);
    header->payload_size = ntohs(header->payload_size);
    switch (header->type) {
        case COMMAND_REGISTER:
        case COMMAND_PING:
        case COMMAND_PONG:
        case COMMAND_INPUT_MIC_BLOW:
        case COMMAND_INPUT_VIBRATE:
            break;
        case COMMAND_INPUT_BUTTON:
            button = (CommandPacketButtonServer*) header->payload;
            button->button = ntohs(button->button);
            break;
        case COMMAND_INPUT_BUTTON_EXTRA:
            button_extra = (CommandPacketButtonExtraServer*) header->payload;
            break;
        case COMMAND_INPUT_JOYSTICK:
            joystick = (CommandPacketJoystickServer*) header->payload;
            joystick->left_x = ntohs(joystick->left_x);
            joystick->left_y = ntohs(joystick->left_y);
            joystick->right_x = ntohs(joystick->right_x);
            joystick->right_y = ntohs(joystick->right_y);
            break;
        case COMMAND_INPUT_TOUCH:
            touch = (CommandPacketTouchServer*) header->payload;
            touch->x = ntohs(touch->x);
            touch->y = ntohs(touch->y);
            touch->width = ntohs(touch->width);
            touch->height = ntohs(touch->height);
            break;
        default:
            break;
    }
}
