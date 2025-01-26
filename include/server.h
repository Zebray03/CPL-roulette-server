#ifndef SERVER_H
#define SERVER_H

#include "game.h"
#include "../util/socket.h"

typedef struct server {
    SOCKET server_socket;
    SOCKET client_socket1;
    SOCKET client_socket2;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct game game;
} Server;

void initialize_server(Server* server);      // 初始化服务器

void accept_clients(Server* server);        // 接受客户端连接

void handle_game(Server* server);           // 处理游戏逻辑

#endif //SERVER_H
