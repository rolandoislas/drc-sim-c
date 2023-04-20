//
// Created by rolando on 5/16/17.
//

#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <zconf.h>
#include "Server.h"
#include "util/logging/Logger.h"
#include "data/Constants.h"
#include "Gamepad.h"
#include "net/server/packet/CommandPacketServer.h"

using namespace std;

int Server::socket_vid;
int Server::socket_aud;
int Server::socket_cmd;
vector<ClientSocket> Server::client_sockets;

Server::Server() {

}

void Server::run() {
    Server server;
    server.print_init();
    server.init_sockets();
    server.running = true;
    while (server.running) {
        server.update_sockets();
        server.check_socket_time();
    }
}

void Server::check_socket_time() {
    for (int sock = 0; sock < client_sockets.size(); ++sock) {
        ClientSocket s = client_sockets[sock];
        if (s.type != COMMAND)
            continue;
        if (time(0) - s.ping_time >= 5)
            remove_client_socket(&s);
    }
}

void Server::update_sockets() {
    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 1;
    FD_ZERO(&read_set);
    FD_SET(socket_vid, &read_set);
    FD_SET(socket_aud, &read_set);
    FD_SET(socket_cmd, &read_set);
    select(socket_cmd + 1, &read_set, NULL, NULL, &time);
    if (FD_ISSET(socket_cmd, &read_set))
        handle_packet(socket_cmd, (PacketHandler *) &command_handler);
    if (FD_ISSET(socket_vid, &read_set)) {
        int fd = accept(socket_vid, NULL, NULL);
        register_client_socket(fd, VIDEO);
    }
    if (FD_ISSET(socket_aud, &read_set)) {
        int fd = accept(socket_aud, NULL, NULL);
        register_client_socket(fd, AUDIO);
    }
}

void Server::print_init() {

}

void Server::init_sockets() {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    // VID
    address.sin_port = htons(PORT_SERVER_VID);
    socket_vid = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(socket_vid, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        Logger::error(Logger::SERVER, "Failed to set socket options for VID socket.");
    if (bind(socket_vid, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::SERVER, "Could not bind VID socket.");
    if (listen(socket_vid, 0) == -1)
        Logger::error(Logger::SERVER, "Failed to listen for incoming connections on VID socket.");
    // AUD
    address.sin_port = htons(PORT_SERVER_AUD);
    socket_aud = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(socket_aud, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        Logger::error(Logger::SERVER, "Failed to set socket options for AUD socket.");
    if (bind(socket_aud, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::SERVER, "Could not bind AUD socket.");
    if (listen(socket_aud, 0) == -1)
        Logger::error(Logger::SERVER, "Failed to listen for incoming connections on AUD socket.");
    // CMD
    address.sin_port = htons(PORT_SERVER_CMD);
    socket_cmd = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(socket_cmd, (const sockaddr *) &address, sizeof(address)) == -1)
        Logger::error(Logger::SERVER, "Could not bind CMD socket.");
}

void Server::broadcast_video(uint8_t *frame, size_t size) {
    broadcast_media(frame, size, VIDEO);
}

void Server::handle_packet(int fd, PacketHandler *handler) {
    unsigned char data[2048];
    sockaddr_in address;
    socklen_t address_size = sizeof(address);
    ssize_t size = recvfrom(fd, &data, sizeof(data), 0, (sockaddr *) &address, &address_size);
    (*handler).update(data, (size_t) size, &address, &address_size);
}

void Server::register_client_socket(in_port_t port, in_addr_t addr, const int type) {
    ClientSocket client_socket;
    client_socket.type = type;
    client_socket.addr = addr;
    client_socket.port = port;
    client_socket.ping_time = time(0);
    client_socket.tries = 2;
    client_sockets.push_back(client_socket);
    char str_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, str_address, INET_ADDRSTRLEN);
    Logger::debug(Logger::SERVER, "Added client socket - type: %d, addr: %s, port: %d", type, str_address, port);
}

void Server::remove_client_socket(ClientSocket *client_socket) {
    if (client_socket->tries < 3) {
        client_socket->tries++;
        return;
    }
    for (int sock = 0; sock < client_sockets.size(); ++sock) {
        if ((client_sockets[sock].addr > 0 and
                client_sockets[sock].port > 0 and
                client_sockets[sock].addr == client_socket->addr and
                client_sockets[sock].port == client_socket->port) or
                client_sockets[sock].fd == client_socket->fd) {
            if (client_socket->type == COMMAND) {
                char str_address[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_socket->addr, str_address, INET_ADDRSTRLEN);
                Logger::debug(Logger::SERVER, "Removed client socket - type: %d, addr: %s, port: %d",
                              client_socket->type,
                              str_address, client_socket->port);
            }
            else {
                Logger::debug(Logger::SERVER, "Removed client socket - type: %d", client_socket->type);
            }
            close(client_socket->fd);
            client_sockets.erase(client_sockets.begin() + sock);
            return;
        }
    }
}

void Server::update_socket_time(in_port_t port, in_addr_t addr) {
    for (int sock = 0; sock < client_sockets.size(); ++sock) {
        if (client_sockets[sock].addr == addr and
            client_sockets[sock].port == port) {
            client_sockets[sock].ping_time = time(0);
            return;
        }
    }
}

void Server::register_client_socket(int fd, const int type) {
    ClientSocket client_socket;
    client_socket.type = type;
    client_socket.fd = fd;
    client_socket.tries = 0;
    client_socket.addr = 0;
    client_socket.port = 0;
    client_sockets.push_back(client_socket);
    Logger::debug(Logger::SERVER, "Added client socket - type %d", type);
}

void Server::broadcast_audio(unsigned char *data, size_t size) {
    broadcast_media(data, size, AUDIO);
}

void Server::broadcast_command(uint16_t command_id) {
    CommandPacketHeaderServer packet = { .type = htons(command_id), .payload_size = htons(0) };
    broadcast_command((unsigned char *)&packet, sizeof(CommandPacketHeaderServer));
}

void Server::broadcast_media(unsigned char *data, size_t size, const int type) {
    vector<ClientSocket> client_sockets_temp(Server::client_sockets);
    unsigned char packet[size + sizeof(PACKET_DELIMITER)];
    memcpy(packet, &PACKET_DELIMITER, sizeof(PACKET_DELIMITER));
    memcpy(packet + sizeof(PACKET_DELIMITER), data, size);
    for (int socket = 0; socket < client_sockets.size(); ++socket) {
        ClientSocket &client_socket = client_sockets[socket];
        if (client_socket.type != type)
            continue;
        size_t remaining = sizeof(PACKET_DELIMITER) + size;
        while (remaining > 0) {
            int offset = (int) (size - remaining + sizeof(PACKET_DELIMITER));
            ssize_t sent = send(client_socket.fd, packet + offset, remaining, MSG_NOSIGNAL);
            if (sent <= 0) {
                remove_client_socket(&client_socket);
                remaining = 0;
            }
            else
                remaining -= sent;
        }
    };
}

void Server::broadcast_command(unsigned char *packet, size_t packet_size) {
    vector<ClientSocket> client_sockets_temp(Server::client_sockets);
    for (int socket = 0; socket < client_sockets.size(); ++socket) {
        ClientSocket &client_socket = client_sockets[socket];
        if (client_socket.type != COMMAND)
            continue;
        size_t remaining = packet_size;
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = client_socket.addr;
        address.sin_port = client_socket.port;
        while (remaining > 0) {
            int offset = (int) (packet_size - remaining);
            ssize_t sent = sendto(socket_cmd, packet + offset, remaining, MSG_NOSIGNAL, (sockaddr *)&address,
                                  sizeof(address));
            if (sent <= 0) {
                remove_client_socket(&client_socket);
                remaining = 0;
            }
            else
                remaining -= sent;
        }
    };
}
