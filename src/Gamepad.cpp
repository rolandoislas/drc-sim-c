//
// Created by rolando on 5/8/17.
//

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "Gamepad.h"
#include "data/Config.h"
#include "util/logging/Logger.h"
#include "data/Constants.h"

using namespace std;

void Gamepad::start() {
    Config::init();
    print_init();
    connect();
    running = true;
    while (running) {
        update();
    }
}

void Gamepad::print_init() {
    Logger::info(Logger::DRC, "Starting backend");
    print_config();
    Logger::info(Logger::DRC, "Waiting for packets");
}

void Gamepad::print_config() {

}

void Gamepad::connect() {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.11");
    // CMD
    address.sin_port = htons(PORT_WII_CMD);
    socket_cmd = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_cmd, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind CMD socket.");
    // AUD
    address.sin_port = htons(PORT_WII_AUD);
    socket_aud = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_aud, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind CMD socket.");
    // VID
    address.sin_port = htons(PORT_WII_VID);
    socket_vid = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_vid, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind CMD socket.");
    // MSG
    address.sin_port = htons(PORT_WII_MSG);
    socket_msg = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_msg, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind CMD socket.");
    // HID
    address.sin_port = htons(PORT_WII_HID);
    socket_hid = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_hid, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind CMD socket.");
}

void Gamepad::update() {
    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 1;
    FD_ZERO(&read_set);
    FD_SET(socket_cmd, &read_set);
    FD_SET(socket_aud, &read_set);
    FD_SET(socket_vid, &read_set);
    FD_SET(socket_msg, &read_set);
    FD_SET(socket_hid, &read_set);
    int amount = select(socket_hid + 1, &read_set, NULL, NULL, &time);
    if (amount > 0 and !received_wii_u_packet) {
        received_wii_u_packet = true;
        Logger::info(Logger::DRC, "Received a Wii U packet");
    }
    if (FD_ISSET(socket_cmd, &read_set)) {
        unsigned char data[2048];
        memset(data, 0, sizeof(data));
        ssize_t size = recv(socket_cmd, &data, sizeof(data), 0);
        command_handler.update(data, (size_t) size);
    }
    if (FD_ISSET(socket_vid, &read_set)) {
        unsigned char data[2048];
        memset(data, 0, sizeof(data));
        ssize_t size = recv(socket_vid, &data, sizeof(data), 0);
        video_handler.update(data, (size_t) size);
    }
}
