//
// Created by rolando on 5/5/17.
//

#ifndef DRC_SIM_C_CONSTANTS_H
#define DRC_SIM_C_CONSTANTS_H


#include <string>
#include "../util/OSUtil.h"

// General
static const std::string NAME = "DRC Sim Server";
static const std::string VERSION = "1.0.2";

// Ports
static const uint16_t PORT_WII_MSG = 50110;
static const uint16_t PORT_WII_VID = 50120;
static const uint16_t PORT_WII_AUD = 50121;
static const uint16_t PORT_WII_HID = 50122;
static const uint16_t PORT_WII_CMD = 50123;
static const uint16_t PORT_SERVER_VID = 50000;
static const uint16_t PORT_SERVER_AUD = 50001;
static const uint16_t PORT_SERVER_CMD = 50002;

// Video Dimensions
static const int WII_VIDEO_WIDTH = 848;
static const int WII_VIDEO_HEIGHT = 480;

// Packet
static const unsigned char PACKET_DELIMITER[5] = {'|', '|', '|', '|', '\n'};

// Path
static const std::string PATH_USER_DATA = std::string(getenv("HOME")) + std::string("/.drc-sim/");
static const std::string PATH_INSTALL = "/opt/drc_sim/";

#endif //DRC_SIM_C_CONSTANTS_H
