#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H

#include "player.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#include "cJSON.h"

cJSON* receive_json_message(SOCKET socket);

void send_game_begin_message(SOCKET socket);

void send_game_state_message(SOCKET socket, bool turn, int round, int real_bullet_num);

void send_item_assign_message(SOCKET socket, int knife_num, int beer_num, int phone_num);

void send_player_state_message(SOCKET socket, Player* player);

void send_item_use_message(SOCKET socket, const char item[]);

void send_player_shoot_message(SOCKET socket, bool is_attack);

void send_shoot_result_message(SOCKET socket, bool is_attack, int damage);

void send_game_over_message(SOCKET socket, bool is_winner);

#endif //NETWORK_CONTROLLER_H
