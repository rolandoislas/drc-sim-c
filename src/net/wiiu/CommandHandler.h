//
// Created by rolando on 5/14/17.
//

#ifndef DRC_SIM_C_COMMANDHANDLER_H
#define DRC_SIM_C_COMMANDHANDLER_H


#include <cstddef>
#include "../PacketHandler.h"

class CommandHandler : PacketHandler {

public:
    void update(unsigned char *packet, size_t size) override;
};


#endif //DRC_SIM_C_COMMANDHANDLER_H
