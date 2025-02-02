#include "../include/server.h"

#include <stdio.h>

#define PORT 8080

void initialize_server(Server *server) {
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

void accept_clients(Server *server) {
    int addrlen = sizeof(server->client_addr);
    printf("Waiting for Player 1 to connect...\n");
    server->demon_socket = accept(server->server_socket, (struct sockaddr *)&server->client_addr, &addrlen);
    printf("Player 1 connected.\n");

    printf("Waiting for Player 2 to connect...\n");
    server->gambler_socket = accept(server->server_socket, (struct sockaddr *)&server->client_addr, &addrlen);
    printf("Player 2 connected.\n");
}
