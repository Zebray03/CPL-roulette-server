#include "socket.h"

#define BUFFER_SIZE 1024

void send_message(SOCKET client, const char *message) {
    send(client, message, strlen(message), 0);
}

void receive_message(SOCKET client, char *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
    recv(client, buffer, BUFFER_SIZE, 0);
}