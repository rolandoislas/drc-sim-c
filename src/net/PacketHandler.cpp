//
// Created by rolando on 5/14/17.
//

#include "PacketHandler.h"

bool PacketHandler::update_seq_id(unsigned int seq_id) {
    bool matched = true;
    if (seq_id_expected == -1)
        seq_id_expected = seq_id;
    else if (seq_id_expected != seq_id)
        matched = false;
    seq_id_expected = (seq_id + 1) & 0x3ff;  // 10 bit number
    return matched;
}

void PacketHandler::update(unsigned char *packet, size_t size) {
    update(packet, size, NULL, NULL);
}
