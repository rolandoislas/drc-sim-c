//
// Created by rolando on 5/8/17.
//

#ifndef DRC_SIM_C_GAMEPAD_H
#define DRC_SIM_C_GAMEPAD_H


#include "net/wiiu/VideoHandlerWiiU.h"
#include "net/wiiu/CommandHandlerWiiU.h"
#include "net/wiiu/AudioHandlerWiiU.h"
#include "net/wiiu/HidHandlerWiiU.h"
#include "net/wiiu/MessageHandlerWiiU.h"

class Gamepad {

public:
    static void run();

    static int socket_msg;
    static int socket_cmd;
    static int socket_aud;
    static int socket_vid;
    static int socket_hid;

    static void sendwiiu(int fd, const void *data, size_t data_size, int port);

    static bool running;
private:
    void print_init();

    void print_config();

    void connect();

    void update();

private:
    fd_set read_set;
    bool received_wii_u_packet;
    VideoHandlerWiiU video_handler;
    CommandHandlerWiiU command_handler;
    AudioHandlerWiiU audio_handler;
    MessageHandlerWiiU message_handler;
    HidHandlerWiiU hid_handler;

    void handle_packet(int fd, PacketHandler *handler);
};


#endif //DRC_SIM_C_GAMEPAD_H
