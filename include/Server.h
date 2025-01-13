#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "Game.h"

#define PORT 8080
#define BUFFER_SIZE 1024

struct Server {
    SOCKET server_socket;
    SOCKET client_socket1;
    SOCKET client_socket2;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct Game game;
};

typedef struct Server SERVER;

void initialize_server(SERVER *server);      // 初始化服务器

void accept_clients(SERVER *server);        // 接受客户端连接

void send_message(SOCKET client, const char *message); // 发送消息

void receive_message(SOCKET client, char *buffer);    // 接收消息

void handle_game(SERVER *server);           // 处理游戏逻辑

#endif //SERVER_H
