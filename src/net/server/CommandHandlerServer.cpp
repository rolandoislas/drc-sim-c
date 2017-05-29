//
// Created by rolando on 5/17/17.
//

#include "CommandHandlerServer.h"
#include "packet/CommandPacketServer.h"
#include "../../Server.h"
#include "../../util/logging/Logger.h"
#include "../../Input.h"

void CommandHandlerServer::update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                            unsigned int *address_size) {
    const CommandPacketServer &command_packet = CommandPacketServer(packet, packet_size);
    Logger::debug(Logger::SERVER, "Command socket received command id %d", command_packet.header->type);
    switch (command_packet.header->type) {
        case CommandPacketServer::COMMAND_REGISTER:
            Server::register_client_socket(from_address->sin_port, from_address->sin_addr.s_addr, Server::COMMAND);
            break;
        case CommandPacketServer::COMMAND_PING:
            pong(from_address, address_size);
            break;
        case CommandPacketServer::COMMAND_PONG:
            break;
        case CommandPacketServer::COMMAND_INPUT_BUTTON:
            Input::set_input_button(command_packet.button->button);
            break;
        case CommandPacketServer::COMMAND_INPUT_BUTTON_EXTRA:
            Input::set_input_button_extra(command_packet.button_extra->extra);
            break;
        case CommandPacketServer::COMMAND_INPUT_JOYSTICK:
            Input::set_input_joystick(command_packet.joystick->left_x, command_packet.joystick->left_y,
                                      command_packet.joystick->right_x, command_packet.joystick->right_y);
            break;
        case CommandPacketServer::COMMAND_INPUT_MIC_BLOW:
            Input::set_input_mic_blow(true);
            break;
        case CommandPacketServer::COMMAND_INPUT_VIBRATE:
            break;
        case CommandPacketServer::COMMAND_INPUT_TOUCH:
            Input::set_input_touch(command_packet.touch->x, command_packet.touch->y, command_packet.touch->width,
                                   command_packet.touch->height);
            break;
        default:
            Logger::debug(Logger::SERVER, "Command packet (id %d) unhandled.", command_packet.header->type);
            break;
    }
}

void CommandHandlerServer::pong(sockaddr_in *address, unsigned int *address_size) {
    Server::update_socket_time(address->sin_port, address->sin_addr.s_addr);
    CommandPacketHeaderServer header;
    header.type = htons(CommandPacketServer::COMMAND_PONG);
    header.payload_size = htons(0);
    ssize_t sent = sendto(Server::socket_cmd, &header, sizeof(CommandPacketHeaderServer), 0, (sockaddr *) address,
                          *address_size);
    if (sent == -1)
        Logger::error(Logger::SERVER, "Error sending pong command.");
}


