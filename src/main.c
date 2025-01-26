#include "../include/server.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    Server server;
    initialize_server(&server);
    while (true) {
        accept_clients(&server);
        initialize_game(&server.game);
        // 赌徒 - 1
        // 恶魔 - 2
        send_message(server.client_socket1, "Welcome, Gambler! You are player 1.\n");
        send_message(server.client_socket2, "Welcome, Demon! You are player 2.\n");

        while (is_game_over(&server.game)) {
            Game* game = &server.game;
            Player* current_player = game->gambler_turn == true ? &game->gambler : &game->demon;
            Player* opponent_player = game->gambler_turn == false ? &game->demon : &game->gambler;
            SOCKET current_socket = game->gambler_turn == true ? server.client_socket1 : server.client_socket2;
            SOCKET opponent_socket = game->gambler_turn == false ? server.client_socket1 : server.client_socket2;
            char current_buffer[BUFFER_SIZE];
            char opponent_buffer[BUFFER_SIZE];

            assign_items(current_player);

            sprintf(current_buffer, "Your turn! You have %d health.\n", current_player->hp);
            send_message(current_socket, current_buffer);
            sprintf(current_buffer, "You have Knife: %d, Beer: %d, Phone: %d.\n",
                    current_player->knife_num, current_player->beer_num, current_player->phone_num);
            send_message(current_socket, current_buffer);

            sprintf(current_buffer, "There are %d real bullets this round.\n", game->pistol.real_bullet_num);
            send_message(current_socket, current_buffer);

            sprintf(opponent_buffer, "Opponent's turn...\n");
            send_message(opponent_socket, opponent_buffer);

            send_message(current_socket, "Enter your action (e.g., use knife, shoot 2):\n");
            receive_message(current_socket, current_buffer);

            char action[16];
            bool is_attack; // 0 - 朝自己开枪
            sscanf(current_buffer, "%s %d", action, &is_attack);

            if (strcmp(action, "use") == 0) {
                char item[16];
                sscanf(current_buffer, "use %s", item);
                if (strcmp(item, "knife") == 0) {
                    use_knife(current_player);
                }
                if (strcmp(item, "beer") == 0) {
                    use_beer(current_player);
                }
                if (strcmp(item, "phone") == 0) {
                    use_phone(current_player, game->pistol);
                }
            }

            if (strcmp(action, "shoot") == 0) {
                Player* source = current_player;
                Player* target = is_attack ? opponent_player : current_player;
                shoot(game, source, target,
                      current_socket, current_buffer,
                      opponent_socket, opponent_buffer);
            }

            if (is_game_over(&server.game)) {
                if (game->gambler.hp <= 0) {
                    send_message(server.client_socket1, "game Over! You lost.\n");
                    send_message(server.client_socket2, "game Over! You won.\n");
                }
                else {
                    send_message(server.client_socket1, "game Over! You won.\n");
                    send_message(server.client_socket2, "game Over! You lost.\n");
                }
            }
        }

        closesocket(server.client_socket1);
        closesocket(server.client_socket2);
    }
    closesocket(server.server_socket);
    WSACleanup();
    return 0;
}
