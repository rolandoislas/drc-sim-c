//
// Created by rolando on 5/14/17.
//

#ifndef DRC_SIM_C_PACKETHANDLER_H
#define DRC_SIM_C_PACKETHANDLER_H


#include <cstddef>

class PacketHandler {

public:
    virtual void update(unsigned char *packet, size_t size)= 0;
};


#endif //DRC_SIM_C_PACKETHANDLER_H
