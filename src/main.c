#include "../include/server.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "network_controller.h"
#include "network_protocol.h"

int main() {
    Server server;
    initialize_server(&server);
    while (true) {
        accept_clients(&server);
        // 恶魔 - 1
        // 赌徒 - 2

        initialize_game(server.game);
        Game* game = server.game;
        SOCKET demon_socket = server.demon_socket;
        SOCKET gambler_socket = server.gambler_socket;
        Player* demon = game->demon;
        Player* gambler = game->gambler;
        Pistol* pistol = game->pistol;
        send_game_begin_message(demon_socket);
        send_game_begin_message(gambler_socket);

        while (!is_game_over(game)) {
            game->round = 1;
            generate_bullets(pistol);
            assign_items(demon);
            send_item_assign_message(server.demon_socket, demon->knife_num, demon->beer_num, demon->phone_num);
            assign_items(gambler);
            send_item_assign_message(server.gambler_socket, gambler->knife_num, gambler->beer_num, gambler->phone_num);

            for (int round = 1; round <= ROUND_NUM; round++) {
                Player* current_player = game->demon_turn == true ? demon : gambler;
                Player* opponent_player = game->demon_turn == false ? gambler : demon;
                SOCKET current_socket = game->demon_turn == true ? demon_socket : gambler_socket;
                SOCKET opponent_socket = game->demon_turn == false ? demon_socket : gambler_socket;

                send_game_state_message(current_socket,true, round, pistol->real_bullet_num);
                send_game_state_message(opponent_socket,false, round, pistol->real_bullet_num);

                send_player_state_message(current_socket, current_player);
                send_player_state_message(opponent_socket, opponent_player);

                cJSON* recv_json = receive_json_message(current_socket);
                NetMessageType recv_type = string_to_type(cJSON_GetObjectItem(recv_json, "type")->valuestring);
                switch (recv_type) {
                    case ITEM_USE: {
                        char* item = cJSON_GetObjectItem(recv_json, "item")->valuestring;
                        if (strcmp(item, "KNIFE") == 0) use_knife(current_player);
                        if (strcmp(item, "BEER") == 0) use_beer(current_player);
                        if (strcmp(item, "PHONE") == 0) use_phone(current_player, pistol);
                    }
                    case PLAYER_SHOOT: {
                        bool is_attack = cJSON_GetObjectItem(recv_json, "is_attack")->valueint; // 0 - 朝自己开枪
                        Player* source = current_player;
                        Player* target = is_attack ? opponent_player : current_player;
                        int damage = shoot(game, source, target);
                        send_shoot_result_message(current_socket, is_attack, damage);
                        send_shoot_result_message(opponent_socket, is_attack, damage);
                    }
                    default: {
                        break;
                    }
                }

                if (is_game_over(game)) {
                    if (gambler->hp <= 0) {
                        send_game_over_message(demon_socket,true);
                        send_game_over_message(gambler_socket,false);
                    }
                    else {
                        send_game_over_message(demon_socket,false);
                        send_game_over_message(gambler_socket,true);
                    }
                    break;
                }
            }
        }

        closesocket(server.demon_socket);
        closesocket(server.gambler_socket);
    }
    closesocket(server.server_socket);
    WSACleanup();
    return 0;
}
