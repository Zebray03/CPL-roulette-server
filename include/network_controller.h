#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H

#include "player.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#include "cJSON.h"
#include "game.h"
#include "network_protocol.h"

typedef struct {
    SOCKET server_socket;
    SOCKET demon_socket;
    SOCKET gambler_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    Game game;
} NetworkController;

void initialize_server(NetworkController* nc);

void accept_clients(NetworkController* nc);

cJSON* receive_json_message(SOCKET socket);

/**
 *
 * @param socket
 * @param type
 * @param data
 * @attention No need to call cJSON_Delete() to release param data
 */
void send_type_json_message(SOCKET socket, NetMessageType type, cJSON* data);

void acknowledge(SOCKET socket);

cJSON* build_game_begin_data();

cJSON* build_game_over_data(bool is_winner);

cJSON* build_game_state_data(bool turn, const Player* rival, int round, int real_bullet_num);

cJSON* build_item_assign_data(int knife_num, int beer_num, int phone_num);

cJSON* build_item_use_data(const char item[]);

cJSON* build_phone_result_data(int index, bool is_true);

cJSON* build_player_shoot_data(bool is_attack);

cJSON* build_player_state_data(const Player* player);

cJSON* build_shoot_result_data(bool is_attack, int damage);

#endif //NETWORK_CONTROLLER_H
