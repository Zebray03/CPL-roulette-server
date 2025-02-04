#include <stdbool.h>

#include "network_controller.h"

int main() {
    NetworkController server;
    initialize_server(&server);
    while (true) {
        accept_clients(&server);
        // 恶魔 - 1
        // 赌徒 - 2

        start_pvp_game(&server.game, server.demon_socket, server.gambler_socket);

        closesocket(server.demon_socket);
        closesocket(server.gambler_socket);
    }
    closesocket(server.server_socket);
    WSACleanup();
    return 0;
}
