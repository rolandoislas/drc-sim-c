//
// Created by rolando on 5/16/17.
//

#ifndef DRC_SIM_C_SERVER_H
#define DRC_SIM_C_SERVER_H


#include <queue>
#include "net/server/CommandHandlerServer.h"
#include "net/PacketHandler.h"
#include "net/server/packet/CommandPacketServer.h"

typedef struct {
    int type;
    int fd;
    uint32_t addr;
    uint16_t port;
    time_t ping_time;
    int tries;
} ClientSocket;

class Server {

public:
    Server();
    static void run();

    static int socket_vid;
    static int socket_aud;
    static int socket_cmd;

    static void broadcast_video(uint8_t *frame, size_t size);

    static const int VIDEO = 0;
    static const int AUDIO = 1;
    static const int COMMAND = 2;

    static void register_client_socket(in_port_t port, in_addr_t addr, const int type);

    static void update_socket_time(in_port_t port, in_addr_t addr);

    static void broadcast_audio(unsigned char *data, size_t size);

    static void broadcast_command(uint16_t command_id);

private:
    bool running;

    void print_init();

    void init_sockets();

    static std::vector<ClientSocket> client_sockets;

    void update_sockets();

    fd_set read_set;
    CommandHandlerServer command_handler;

    void handle_packet(int fd, PacketHandler *handler);

    static void remove_client_socket(ClientSocket *client_socket);

    void check_socket_time();

    static void register_client_socket(int fd, const int type);

    static void broadcast_media(unsigned char *data, size_t size, const int type);

    static void broadcast_command(unsigned char *packet, size_t packet_size);
};


#endif //DRC_SIM_C_SERVER_H
