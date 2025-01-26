#ifndef PLAYER_H
#define PLAYER_H

#include "pistol.h"

#include <stdbool.h>

typedef struct player {
    int hp;
    int knife_num;
    int beer_num;
    int phone_num;
    bool knife_use;
    bool beer_use;
    bool phone_use;
} Player;

void initialize_player(Player* player);

void assign_items(Player* player);

void calculate_damage(Player* target_player, int damage);

void use_knife(Player* player);

void use_beer(Player* player);

void use_phone(Player* player, Pistol pistol);

#endif //PLAYER_H
