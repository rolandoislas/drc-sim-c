//
// Created by rolando on 5/14/17.
//

#include <arpa/inet.h>
#include <json.h>
#include <assert.h>
#include <sstream>
#include "CommandHandlerWiiU.h"
#include "../../util/logging/Logger.h"
#include "../../Gamepad.h"
#include "../../data/Constants.h"
#include "../../util/Args.h"
#include "../../util/Resource.h"
#include "packet/CommandPacketWiiU.h"

using namespace std;

char CommandHandlerWiiU::hex_array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

CommandHandlerWiiU::CommandHandlerWiiU() {
    // Get command responses
    char path[100];
    send_blank_response = strcmp(Args::region, "none") == 0;
    sprintf(path, "command/%s.json", send_blank_response ? "na" : Args::region);
    Resource command_json(path, false);
    if (!command_json.exists()) {
        Logger::info(Logger::DRC, "Specified region not found. Using default values instead.");
        send_blank_response = true;
        command_json = Resource("command/na.json");
    }
    char *json_file = new char[1000000];
    int json_file_size = command_json.as_bytes(json_file);
    command_responses = json_parse(json_file, (size_t) json_file_size);
    // Validate JSON
    if (command_responses->type != json_type::json_object)
        Logger::error(Logger::DRC, "Command JSON malformed: root is not an object.");
    if (string(command_responses->u.object.values[0].name) != "0")
        Logger::error(Logger::DRC, "Command JSON malformed: expected cmd 0 in root object.");
    if (string(command_responses->u.object.values[1].name) != "1")
        Logger::error(Logger::DRC, "Command JSON malformed: expected cmd 1 in root object.");
    if (command_responses->u.object.values[0].value->type != json_type::json_object)
        Logger::error(Logger::DRC, "Command JSON malformed: expected cmd 0 to be an object.");
    if (command_responses->u.object.values[1].value->type != json_type::json_string)
        Logger::error(Logger::DRC, "Command JSON malformed: expected cmd 1 to be a string.");
}

void CommandHandlerWiiU::update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                            unsigned int *address_size) {
    const CommandPacketWiiU &command_packet = CommandPacketWiiU(packet, packet_size);
    if (command_packet.header->packet_type != command_packet.PT_REQ and
            command_packet.header->packet_type != command_packet.PT_RESP)
        return;
    if (Logger::is_level_enabled(Logger::FINER))
        Logger::finer(Logger::DRC, "CMD (%d): %s", command_packet.header->cmd_id, Logger::to_hex(packet, packet_size));
    ack(&command_packet);
    switch (command_packet.header->cmd_id) {
        case 0:
            handle_cmd0(&command_packet);
            break;
        case 1:
            handle_cmd1(&command_packet);
            break;
        case 2:
            handle_cmd2(&command_packet);
            break;
        default:break;
    }
}

void CommandHandlerWiiU::ack(const CommandPacketWiiU *packet) {
    CommandPacketHeaderWiiU response;
#if __BYTE_ORDER == BIG_ENDIAN
    // TODO big endian
#else
    response.packet_type = packet->header->packet_type == packet->PT_REQ ? packet->PT_REQ_ACK : packet->PT_RESP_ACK;
    response.cmd_id = packet->header->cmd_id;
    response.payload_size = 0;
    response.seq_id = packet->header->seq_id;
#endif
    Gamepad::sendwiiu(Gamepad::socket_cmd, &response, sizeof(response) - sizeof(response.payload), PORT_WII_CMD);
}

void CommandHandlerWiiU::handle_cmd0(const CommandPacketWiiU *packet) {
    Logger::debug(Logger::DRC, "CMD0: %d %d", packet->header_cmd0->id_primary, packet->header_cmd0->id_secondary);
    string id_primary = to_string(packet->header_cmd0->id_primary);
    string id_secondary = to_string(packet->header_cmd0->id_secondary);

    json_value *obj_cmd0 = command_responses->u.object.values[0].value;
    json_value *obj_primary_id = NULL;
    json_value *str_secondary_id = NULL;
    for (int id = 0; id < obj_cmd0->u.object.length; ++id) {
        if (obj_cmd0->u.object.values[id].value->type != json_type::json_object)
            Logger::error(Logger::DRC, "Command JSON malformed: expected cmd 0 %d to be an object.", id);
        if (obj_cmd0->u.object.values[id].name == id_primary)
            obj_primary_id = obj_cmd0->u.object.values[id].value;
    }
    if (obj_primary_id != NULL) {
        for (int id = 0; id < obj_primary_id->u.object.length; ++id) {
            if (obj_primary_id->u.object.values[id].value->type != json_type::json_string)
                Logger::error(Logger::DRC, "Command JSON malformed: expected cmd 0 %s %d to be a string.",
                              id_primary.c_str(), id);
            if (obj_primary_id->u.object.values[id].name == id_secondary)
                str_secondary_id = obj_primary_id->u.object.values[id].value;
        }
    }

    if (str_secondary_id == NULL) {
        Logger::debug(Logger::DRC, "Unhandled packet CMD0: %s %s", id_primary.c_str(), id_secondary.c_str());
        return;
    }

    char *payload_str = str_secondary_id->u.string.ptr;
    int payload_str_size = str_secondary_id->u.string.length;
    unsigned char *payload = new unsigned char[payload_str_size / 2];
    int payload_size = hex_str_to_bytes(payload_str, payload_str_size, payload);
    send_response_cmd0(packet, payload, payload_size, packet->CMD0_OK);
    delete [] payload;
}

void CommandHandlerWiiU::handle_cmd1(const CommandPacketWiiU *packet) {
    CommandPacketHeaderWiiU *header_raw = (CommandPacketHeaderWiiU *)packet->packet_raw;
    char *payload_str = command_responses->u.object.values[1].value->u.string.ptr;
    int payload_str_size = command_responses->u.object.values[1].value->u.string.length;
    unsigned char *payload = new unsigned char[payload_str_size / 2];
    int payload_size = hex_str_to_bytes(payload_str, payload_str_size, payload);
    send_response(header_raw, NULL, payload, payload_size);
}

void CommandHandlerWiiU::handle_cmd2(const CommandPacketWiiU *packet) {
    CommandPacketHeaderWiiU *header_raw = (CommandPacketHeaderWiiU *)packet->packet_raw;
    send_response(header_raw, NULL, NULL, 0);
}

void CommandHandlerWiiU::send_response_cmd0(const CommandPacketWiiU *packet, unsigned char *payload, int payload_size,
                                            const uint16_t result) {
    assert(packet->header->cmd_id == 0);
    CommandPacketHeaderWiiU *header_raw = (CommandPacketHeaderWiiU *)packet->packet_raw;
    CommandPacketHeaderWiiUCommandZero *header_raw0 = (CommandPacketHeaderWiiUCommandZero *)header_raw->payload;
    header_raw0->flags = (uint8_t) (((packet->header_cmd0->flags >> 3) & 0xfc) | 1);
    header_raw0->error_code = htons(result);
    header_raw0->payload_size = htons((uint16_t) payload_size);
    send_response(header_raw, header_raw0, payload, payload_size);
}

void CommandHandlerWiiU::send_response(CommandPacketHeaderWiiU *header, void *header_cmd,
                                       unsigned char *payload, int payload_size) {
    if (send_blank_response)
        memset(payload, 0, (size_t) payload_size);
    send_command(header, header_cmd, payload, payload_size, CommandPacketWiiU::PT_RESP);
}

void CommandHandlerWiiU::send_command(CommandPacketHeaderWiiU *header, void *header_cmd,
                                      unsigned char *payload, int payload_size, uint16_t packet_type) {
    int full_payload_size = payload_size;
    int header_cmd_size = 0;
    if (header->cmd_id == 0) {
        header_cmd_size = sizeof(CommandPacketHeaderWiiUCommandZero) -
                             sizeof(CommandPacketHeaderWiiUCommandZero::payload);
    }
    full_payload_size += header_cmd_size;
#if __BYTE_ORDER == LITTLE_ENDIAN
    header->packet_type = packet_type;
    header->payload_size = (uint16_t) full_payload_size;
#else
    // TODO big endian
#endif
    if (header_cmd_size > 0)
        memcpy(header->payload, header_cmd, (size_t) header_cmd_size);
    if (payload_size > 0)
        memcpy(header->payload + header_cmd_size, payload, (size_t) payload_size);
    int packet_size = sizeof(CommandPacketHeaderWiiU) - sizeof(CommandPacketHeaderWiiU::payload) + full_payload_size;
    Gamepad::sendwiiu(Gamepad::socket_cmd, header, (size_t) packet_size, PORT_WII_CMD);
}

int CommandHandlerWiiU::hex_char_to_int(char hex_char) {
    for (int hex_pos = 0; hex_pos < sizeof(hex_array); ++hex_pos)
        if (hex_array[hex_pos] == toupper(hex_char))
            return hex_pos;
    return 0;
}

int CommandHandlerWiiU::hex_str_to_bytes(char *str, int str_size, unsigned char *bytes) {
    for (int byte = 0; byte < str_size; byte += 2) {
        int one = hex_char_to_int(str[byte]);
        int two = hex_char_to_int(str[byte + 1]);
        bytes[byte / 2] = (unsigned char) ((one << 4) + two);
    }
    return str_size / 2;
}
