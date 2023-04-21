//
// Created by rolando on 5/14/17.
//

#include "PacketHandler.h"

bool PacketHandler::update_seq_id(unsigned int seq_id) {
    bool matched = seq_id_expected == seq_id;
    seq_id_expected = ++seq_id & 0x3ff;  // 10 bit number
    return matched;
}

void PacketHandler::update(unsigned char *packet, size_t size) {
    update(packet, size, NULL, NULL);
}
