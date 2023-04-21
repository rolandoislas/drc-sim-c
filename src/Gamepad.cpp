//
// Created by rolando on 5/8/17.
//

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "Gamepad.h"
#include "util/logging/Logger.h"
#include "data/Constants.h"
#include "util/Args.h"

using namespace std;

int Gamepad::socket_msg;
int Gamepad::socket_cmd;
int Gamepad::socket_aud;
int Gamepad::socket_vid;
int Gamepad::socket_hid;
bool Gamepad::running;
AudioHandlerWiiU Gamepad::audio_handler;

void Gamepad::run() {
    Gamepad gamepad;
    gamepad.print_init();
    gamepad.connect();
    gamepad.running = true;
    while (gamepad.running) {
        gamepad.update();
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
    address.sin_addr.s_addr = INADDR_ANY;
    // CMD
    address.sin_port = htons(PORT_WII_CMD);
    socket_cmd = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_cmd, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind CMD socket.");
    // AUD
    address.sin_port = htons(PORT_WII_AUD);
    socket_aud = socket(AF_INET, SOCK_DGRAM, 0);
    if (!Args::disable_audio)
        if (bind(socket_aud, (const sockaddr *) &address, sizeof(address)) == -1)
            Logger::error(Logger::DRC, "Could not bind AUD socket.");
    // VID
    address.sin_port = htons(PORT_WII_VID);
    socket_vid = socket(AF_INET, SOCK_DGRAM, 0);
    if (!Args::disable_video)
        if (bind(socket_vid, (const sockaddr *) &address, sizeof(address)) == -1)
            Logger::error(Logger::DRC, "Could not bind VID socket.");
    // MSG
    address.sin_port = htons(PORT_WII_MSG);
    socket_msg = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_msg, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind MSG socket.");
    // HID
    address.sin_port = htons(PORT_WII_HID);
    socket_hid = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_hid, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::DRC, "Could not bind HID socket.");
}

void Gamepad::update() {
    FD_ZERO(&read_set);
    FD_SET(socket_cmd, &read_set);
    FD_SET(socket_aud, &read_set);
    FD_SET(socket_vid, &read_set);
    FD_SET(socket_msg, &read_set);
    FD_SET(socket_hid, &read_set);
    if (select(socket_hid + 1, &read_set, NULL, NULL, NULL) > 0) {
        if (!received_wii_u_packet) {
            received_wii_u_packet = true;
            Logger::info(Logger::DRC, "Received a Wii U packet");
        }
        if (FD_ISSET(socket_cmd, &read_set))
            handle_packet(socket_cmd, (PacketHandler *) &command_handler);
        if (FD_ISSET(socket_vid, &read_set))
            handle_packet(socket_vid, (PacketHandler *) &video_handler);
        if (FD_ISSET(socket_aud, &read_set))
            handle_packet(socket_aud, (PacketHandler *) &audio_handler);
        if (FD_ISSET(socket_msg, &read_set))
            handle_packet(socket_msg, (PacketHandler *) &message_handler);
        if (FD_ISSET(socket_hid, &read_set))
            handle_packet(socket_hid, (PacketHandler *) &hid_handler);
}

void Gamepad::handle_packet(int fd, PacketHandler *handler) {
    static unsigned char data[2048];
    ssize_t size = recv(fd, &data, sizeof(data), 0);
    (*handler).update(data, (size_t) size);
}

void Gamepad::sendwiiu(int fd, const void *data, size_t data_size, int port) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.10");
    address.sin_port = htons((uint16_t) (port - 100));
    ssize_t sent = sendto(fd, data, data_size, 0, (sockaddr *) &address, sizeof(address));
    if (sent == -1)
        Logger::error(Logger::DRC, "Failed to send to Wii U socket: fd - %d; port - %d", fd, port);
}
