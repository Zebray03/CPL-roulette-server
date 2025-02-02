#include "network_controller.h"
#include "network_protocol.h"
#include "pistol.h"

#define MAX_PACKET_SIZE 1024

void send_json_message(SOCKET socket, const cJSON* json) {
    char* json_string = cJSON_PrintUnformatted(json);
    send(socket, json_string, strlen(json_string), 0);
    free(json_string);
}

cJSON* receive_json_message(SOCKET socket) {
    char buffer[MAX_PACKET_SIZE];
    int recv_size = recv(socket, buffer, sizeof(buffer) - 1, 0);
    buffer[recv_size] = '\0';

    cJSON* json_message = cJSON_Parse(buffer);
    if (!json_message) {
        exit(1);
    }

    return json_message;
}

void send_game_begin_message(SOCKET socket) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(GAME_BEGIN));
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_game_state_message(SOCKET socket, const bool turn, const int round, const int real_bullet_num) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(GAME_STATE));
    cJSON_AddBoolToObject(msg, "turn", turn);
    cJSON_AddNumberToObject(msg, "round", round);
    cJSON_AddNumberToObject(msg, "real_bullet_num", real_bullet_num);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_item_assign_message(SOCKET socket, const int knife_num, const int beer_num, const int phone_num) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(ITEM_ASSIGN));
    cJSON_AddNumberToObject(msg, "knife_num", knife_num);
    cJSON_AddNumberToObject(msg, "beer_num", beer_num);
    cJSON_AddNumberToObject(msg, "phone_num", phone_num);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_player_state_message(SOCKET socket, Player* player) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(PLAYER_STATE));
    cJSON_AddNumberToObject(msg, "hp", player->hp);
    cJSON* json_bullet = cJSON_CreateArray();
    for (int i = 0; i < ROUND_NUM; i++) {
        cJSON_AddItemToArray(json_bullet, cJSON_CreateNumber(player->bullets[i]));
    }
    cJSON_AddItemToObject(msg, "bullets", json_bullet);
    cJSON_AddNumberToObject(msg, "knife_num", player->knife_num);
    cJSON_AddNumberToObject(msg, "beer_num", player->beer_num);
    cJSON_AddNumberToObject(msg, "phone_num", player->phone_num);
    cJSON_AddBoolToObject(msg, "knife_use", player->knife_use);
    cJSON_AddBoolToObject(msg, "beer_use", player->beer_use);
    cJSON_AddBoolToObject(msg, "phone_use", player->phone_use);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_item_use_message(SOCKET socket, const char item[]) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(ITEM_USE));
    cJSON_AddStringToObject(msg, "item", item);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_player_shoot_message(SOCKET socket, const bool is_attack) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(PLAYER_SHOOT));
    cJSON_AddBoolToObject(msg, "is_attack", is_attack);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_shoot_result_message(SOCKET socket, bool is_attack, int damage) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(SHOOT_RESULT));
    cJSON_AddBoolToObject(msg, "is_attack", is_attack);
    cJSON_AddNumberToObject(msg, "damage", damage);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

void send_game_over_message(SOCKET socket, const bool is_winner) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(GAME_OVER));
    cJSON_AddBoolToObject(msg, "is_winner", is_winner);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}
