//
// Created by rolando on 5/8/17.
//

#ifndef DRC_SIM_C_VIDEOHANDLER_H
#define DRC_SIM_C_VIDEOHANDLER_H


#include "../PacketHandler.h"

class VideoHandler : PacketHandler {

public:
    void update(unsigned char *packet, size_t size) override;
};


#endif //DRC_SIM_C_VIDEOHANDLER_H
