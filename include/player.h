#ifndef PLAYER_H
#define PLAYER_H

#include "pistol.h"

#include <stdbool.h>

#include "cJSON.h"

#define HP_MAX 3

typedef struct {
    int hp;
    bool is_demon;
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

void initialize_player(Player* player, bool is_demon);

void assign_items(Player* player);

void calculate_damage(Player* target_player, int damage);

/**
 *
 * @param player
 * @attention 合法性检查由客户端负责
 */
void use_knife(Player* player);

/**
 *
 * @param player
 * @attention 合法性检查由客户端负责
 */
void use_beer(Player* player);

/**
 *
 * @param player
 * @param pistol
 * @attention 合法性检查由客户端负责
 */
cJSON* use_phone(Player* player, const Pistol* pistol);

#endif //PLAYER_H
