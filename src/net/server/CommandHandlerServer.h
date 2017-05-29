//
// Created by rolando on 5/17/17.
//

#ifndef DRC_SIM_C_COMMANDHANDLER_SERVER_H
#define DRC_SIM_C_COMMANDHANDLER_SERVER_H


#include "../PacketHandler.h"

class CommandHandlerServer : PacketHandler {
public:
    void update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                unsigned int *address_size) override;

    void pong(sockaddr_in *address, unsigned int *address_size);
};


#endif //DRC_SIM_C_COMMANDHANDLER_SERVER_H
