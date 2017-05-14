//
// Created by rolando on 5/5/17.
//

#ifndef DRC_SIM_C_CONSTANTS_H
#define DRC_SIM_C_CONSTANTS_H


#include <string>

// General
static const std::string NAME = "DRC Sim Server";
static const std::string VERSION = "1.0";

// Ports
static uint16_t PORT_WII_MSG = 50110;
static uint16_t PORT_WII_VID = 50120;
static uint16_t PORT_WII_AUD = 50121;
static uint16_t PORT_WII_HID = 50122;
static uint16_t PORT_WII_CMD = 50123;
static uint16_t PORT_SERVER_VID = 50000;
static uint16_t PORT_SERVER_AUD = 50001;
static uint16_t PORT_SERVER_CMD = 50002;

// Path
static const std::string PATH_ROOT = std::string(getenv("HOME")) + std::string("/.drc-sim/");

#endif //DRC_SIM_C_CONSTANTS_H
