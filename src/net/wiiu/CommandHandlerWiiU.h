//
// Created by rolando on 5/14/17.
//

#ifndef DRC_SIM_C_COMMANDHANDLER_WII_U_H
#define DRC_SIM_C_COMMANDHANDLER_WII_U_H


#include <cstddef>
#include <json.h>
#include "../PacketHandler.h"
#include "packet/CommandPacketWiiU.h"

class CommandHandlerWiiU : PacketHandler {
    void update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                unsigned int *address_size) override;

public:
    CommandHandlerWiiU();

    void ack(const CommandPacketWiiU *packet);

    void handle_cmd0(const CommandPacketWiiU *packet);

    void handle_cmd1(const CommandPacketWiiU *packet);

    void handle_cmd2(const CommandPacketWiiU *packet);

private:
    void send_response_cmd0(const CommandPacketWiiU *packet, unsigned char *payload, int payload_size, const uint16_t result);

    void send_response(CommandPacketHeaderWiiU *header, void *header_cmd, unsigned char *payload, int payload_size);

    void send_command(CommandPacketHeaderWiiU *header, void *header_cmd,
                      unsigned char *payload,
                      int payload_size, uint16_t packet_type);

    int hex_char_to_int(char hex_char);

    static const char hex_array[16];
    json_value *command_responses;

    int hex_str_to_bytes(char *str, int str_size, unsigned char *bytes);

    bool send_blank_response;
};


#endif //DRC_SIM_C_COMMANDHANDLER_WII_U_H
