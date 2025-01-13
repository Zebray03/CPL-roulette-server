#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/Server.h"


int main() {
    SERVER server;
    initialize_server(&server);
    while (true) {
        accept_clients(&server);
        initialize_game(&server.game);
        send_message(server.client_socket1, "Welcome, Gambler! You are Player 1.\n");
        send_message(server.client_socket2, "Welcome, Demon! You are Player 2.\n");
        while (is_game_over(&server.game)) {
            //todo

        }

        closesocket(server.client_socket1);
        closesocket(server.client_socket2);
    }
    closesocket(server.server_socket);
    WSACleanup();
    return 0;
}

