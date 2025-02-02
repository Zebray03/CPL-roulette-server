#ifndef PLAYER_H
#define PLAYER_H

#include "pistol.h"

#include <stdbool.h>

typedef struct {
    int hp;
    int bullets[ROUND_NUM];  // 1 - 实弹，0 - 未知，-1 - 虚弹）
    int knife_num;
    int beer_num;
    int phone_num;
    bool knife_use;
    bool beer_use;
    bool phone_use;
} Player;

typedef enum {
    KNIFE,
    BEER,
    PHONE,
} Item;

void initialize_player(Player* player);

void assign_items(Player* player);

void calculate_damage(Player* target_player, int damage);

void use_knife(Player* player);

void use_beer(Player* player);

void use_phone(Player* player, Pistol* pistol);

#endif //PLAYER_H
