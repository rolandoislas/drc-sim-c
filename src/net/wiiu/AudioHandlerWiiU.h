//
// Created by rolando on 5/17/17.
//

#ifndef DRC_SIM_C_AUDIOHANDLER_WII_U_H
#define DRC_SIM_C_AUDIOHANDLER_WII_U_H


#include "../PacketHandler.h"

class AudioHandlerWiiU : PacketHandler {

public:
    AudioHandlerWiiU();

private:
    void update(unsigned char *packet, size_t packet_size, sockaddr_in *from_address,
                unsigned int *address_size) override;

    void send_mic_blow();

    unsigned char rand_audio[512];
};


#endif //DRC_SIM_C_AUDIOHANDLER_WII_U_H
