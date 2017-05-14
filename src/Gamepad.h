//
// Created by rolando on 5/8/17.
//

#ifndef DRC_SIM_C_GAMEPAD_H
#define DRC_SIM_C_GAMEPAD_H


#include "net/wiiu/VideoHandler.h"
#include "net/wiiu/CommandHandler.h"

class Gamepad {

public:
    void start();

    void print_init();

    void print_config();

    void connect();

    void update();

private:
    fd_set read_set;
    int socket_cmd;
    int socket_aud;
    int socket_vid;
    int socket_msg;
    int socket_hid;
    VideoHandler video_handler;
    bool running;
    bool received_wii_u_packet;
    CommandHandler command_handler;
};


#endif //DRC_SIM_C_GAMEPAD_H
