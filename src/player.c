#include "../include/player.h"

#include <stdio.h>
#include <stdlib.h>

#define HP_MAX 3

void initialize_player(Player* player) {
    player->hp = HP_MAX;
    player->knife_num = 0;
    player->beer_num = 0;
    player->phone_num = 0;
    player->knife_use = false;
    player->beer_use = false;
    player->phone_use = false;
}

void assign_items(Player* player) {
    player->knife_num = rand() % 2;
    player->beer_num = rand() % 2;
    player->phone_num = rand() % 2;
}

void calculate_damage(Player* target_player, int damage) {
    target_player->hp -= damage;
    if (target_player->hp < 0) {
        target_player->hp = 0;
    }
}

void use_knife(Player* player) {
    if (player->knife_num == 0) {
        printf("don't have knife\n");
    }
    else if (player->knife_use) {
        printf("have used knife already\n");
    }
    else {
        player->knife_num--;
        player->knife_use = true;
    }
}

void use_beer(Player* player) {
    if (player->beer_num == 0) {
        printf("don't have beer\n");
    }
    else if (player->beer_use) {
        printf("have used beer already\n");
    }
    else {
        player->hp += 2;
        if (player->hp > HP_MAX) {
            player->hp = HP_MAX;
        }
        player->beer_num--;
        player->beer_use = true;
    }
}

// 这里“任意一发”理解为每一发子弹
void use_phone(Player* player, Pistol* pistol) {
    if (player->phone_num == 0) {
        printf("don't have phone\n");
    }
    else if (player->phone_use) {
        printf("have used phone already\n");
    }
    else {
        printf("the info is as followed:\n");
        for (int i = 0; i < pistol->bullet_remaining_num; i++) {
            if (pistol->is_real_bullet[5 - pistol->bullet_remaining_num + i]) {
                printf("1\n");
            }
            else {
                printf("0\n");
            }
        }
        player->phone_num--;
        player->phone_use = true;
    }
}
