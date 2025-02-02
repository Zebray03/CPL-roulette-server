#ifndef SERVER_H
#define SERVER_H

#include "game.h"

typedef struct server {
    SOCKET server_socket;
    SOCKET demon_socket;
    SOCKET gambler_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct game* game;
} Server;

void initialize_server(Server* server);      // 初始化服务器

void accept_clients(Server* server);        // 接受客户端连接

#endif //SERVER_H
