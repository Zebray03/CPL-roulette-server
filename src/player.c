#include "player.h"
#include "network_controller.h"

#include <stdlib.h>

void initialize_player(Player* player, bool is_demon) {
    player->hp = HP_MAX;
    player->is_demon = is_demon;
    for (int i = 0; i < ROUND_NUM; i++) {
        player->bullets[i] = 0;
    }
    player->knife_num = 0;
    player->beer_num = 0;
    player->phone_num = 0;
    player->knife_use = false;
    player->beer_use = false;
    player->phone_use = false;
}

void assign_items(Player* player) {
    player->knife_num = rand() % 3;
    player->beer_num = rand() % 3;
    player->phone_num = rand() % 3;
}

void calculate_damage(Player* target_player, int damage) {
    target_player->hp -= damage;
    if (target_player->hp < 0) {
        target_player->hp = 0;
    }
}

void use_knife(Player* player) {
    player->knife_num--;
    player->knife_use = true;
}

void use_beer(Player* player) {
    player->hp += 2;
    if (player->hp > HP_MAX) {
        player->hp = HP_MAX;
    }

    player->beer_num--;
    player->beer_use = true;
}

cJSON* use_phone(Player* player, const Pistol* pistol) {
    int unknown_num = 0;
    int* unknown_index = malloc(sizeof(int) * 5);
    for (int i = 0; i < ROUND_NUM; i++) {
        if (player->bullets[i] == 0) {
            unknown_index[unknown_num] = i;
            unknown_num++;
        }
    }
    int index = unknown_index[rand() % unknown_num];
    bool is_true = pistol->is_real_bullet[index] == 1;
    free(unknown_index);

    player->bullets[index] = is_true ? 1 : -1;
    player->phone_num--;
    player->phone_use = true;

    return build_phone_result_data(index, is_true);
}
