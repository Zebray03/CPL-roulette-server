#include "../include/Server.h"

void initialize_server(SERVER *server) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(1);
    }

    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(PORT);

    if (bind(server->server_socket, (struct sockaddr *)&server->server_addr, sizeof(server->server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server->server_socket);
        WSACleanup();
        exit(1);
    }

    if (listen(server->server_socket, 2) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server->server_socket);
        WSACleanup();
        exit(1);
    }
}

void accept_clients(SERVER *server) {
    int addrlen = sizeof(server->client_addr);
    printf("Waiting for Player 1 to connect...\n");
    server->client_socket1 = accept(server->server_socket, (struct sockaddr *)&server->client_addr, &addrlen);
    printf("Player 1 connected.\n");

    printf("Waiting for Player 2 to connect...\n");
    server->client_socket2 = accept(server->server_socket, (struct sockaddr *)&server->client_addr, &addrlen);
    printf("Player 2 connected.\n");
}

void send_message(SOCKET client, const char *message) {
    send(client, message, strlen(message), 0);
}

void receive_message(SOCKET client, char *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
    recv(client, buffer, BUFFER_SIZE, 0);
}

void handle_game(SERVER *server) {
    // 使用 Game 类的逻辑管理整个游戏
}