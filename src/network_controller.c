#include "network_controller.h"

#include <stdio.h>

#include "pistol.h"

#define PORT 8080
#define MAX_PACKET_SIZE 1024

void initialize_server(NetworkController* nc) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(1);
    }

    nc->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (nc->server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    nc->server_addr.sin_family = AF_INET;
    nc->server_addr.sin_addr.s_addr = INADDR_ANY;
    nc->server_addr.sin_port = htons(PORT);

    if (bind(nc->server_socket, (struct sockaddr*)&nc->server_addr, sizeof(nc->server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(nc->server_socket);
        WSACleanup();
        exit(1);
    }

    if (listen(nc->server_socket, 2) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(nc->server_socket);
        WSACleanup();
        exit(1);
    }
}

void accept_clients(NetworkController* nc) {
    int addrlen = sizeof(nc->client_addr);
    printf("Waiting for Player 1 to connect...\n");
    nc->demon_socket = accept(nc->server_socket, (struct sockaddr*)&nc->client_addr, &addrlen);
    printf("Player 1 connected.\n");

    printf("Waiting for Player 2 to connect...\n");
    nc->gambler_socket = accept(nc->server_socket, (struct sockaddr*)&nc->client_addr, &addrlen);
    printf("Player 2 connected.\n");
}

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

void send_type_json_message(SOCKET socket, const NetMessageType type, cJSON* data) {
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "type", type_to_string(type));
    cJSON_AddItemToObject(msg, "data", data);
    send_json_message(socket, msg);
    cJSON_Delete(msg);
}

cJSON* build_acknowledge_data() {
    cJSON* data = cJSON_CreateObject();

    return data;
}

void acknowledge(SOCKET socket) {
    cJSON* acknowledge_data = build_acknowledge_data();
    send_type_json_message(socket, ACKNOWLEDGE, acknowledge_data);
}

cJSON* build_game_begin_data() {
    cJSON* data = cJSON_CreateObject();

    return data;
}

cJSON* build_game_over_data(const bool is_winner) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddBoolToObject(data, "is_winner", is_winner);

    return data;
}

cJSON* build_game_state_data(const bool turn, const Player* rival, const int round, const int real_bullet_num) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddBoolToObject(data, "turn", turn);
    cJSON_AddNumberToObject(data, "rival_hp", rival->hp);
    cJSON_AddNumberToObject(data, "rival_knife_num", rival->knife_num);
    cJSON_AddNumberToObject(data, "rival_beer_num", rival->beer_num);
    cJSON_AddNumberToObject(data, "rival_phone_num", rival->phone_num);
    cJSON_AddNumberToObject(data, "round", round);
    cJSON_AddNumberToObject(data, "real_bullet_num", real_bullet_num);

    return data;
}

cJSON* build_item_assign_data(const int knife_num, const int beer_num, const int phone_num) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddNumberToObject(data, "knife_num", knife_num);
    cJSON_AddNumberToObject(data, "beer_num", beer_num);
    cJSON_AddNumberToObject(data, "phone_num", phone_num);

    return data;
}

cJSON* build_item_use_data(const char item[]) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddStringToObject(data, "item", item);

    return data;
}

cJSON* build_phone_result_data(const int index, const bool is_true) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddNumberToObject(data, "index", index);
    cJSON_AddNumberToObject(data, "is_true", is_true);

    return data;
}

cJSON* build_player_shoot_data(const bool is_attack) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddBoolToObject(data, "is_attack", is_attack);

    return data;
}

cJSON* build_player_state_data(const Player* player) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddNumberToObject(data, "hp", player->hp);
    cJSON_AddBoolToObject(data, "is_demon", player->is_demon ? 1 : 0);
    cJSON* json_bullet = cJSON_CreateArray();
    for (int i = 0; i < ROUND_NUM; i++) {
        cJSON_AddItemToArray(json_bullet, cJSON_CreateNumber(player->bullets[i]));
    }
    cJSON_AddItemToObject(data, "bullets", json_bullet);
    cJSON_AddNumberToObject(data, "knife_num", player->knife_num);
    cJSON_AddNumberToObject(data, "beer_num", player->beer_num);
    cJSON_AddNumberToObject(data, "phone_num", player->phone_num);
    cJSON_AddBoolToObject(data, "knife_use", player->knife_use);
    cJSON_AddBoolToObject(data, "beer_use", player->beer_use);
    cJSON_AddBoolToObject(data, "phone_use", player->phone_use);

    return data;
}

cJSON* build_shoot_result_data(const bool is_attack, const int damage) {
    cJSON* data = cJSON_CreateObject();

    cJSON_AddBoolToObject(data, "is_attack", is_attack);
    cJSON_AddNumberToObject(data, "damage", damage);

    return data;
}
