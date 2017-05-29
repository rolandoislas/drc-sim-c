//
// Created by rolando on 5/17/17.
//

#ifndef DRC_SIM_C_MESSAGEHANDLER_WII_U_H
#define DRC_SIM_C_MESSAGEHANDLER_WII_U_H


#include "../PacketHandler.h"

class MessageHandlerWiiU : PacketHandler {
    void update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                unsigned int *address_size) override;
};


#endif //DRC_SIM_C_MESSAGEHANDLER_WII_U_H
