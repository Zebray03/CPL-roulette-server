#include "../include/Player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_player(PLAYER *player) {
    player->hp = 3;
    player->knife_num = 0;
    player->beer_num = 0;
    player->phone_num = 0;
}

void assign_items(PLAYER *player) {
    player->knife_num = rand() % 2;
    player->beer_num = rand() % 2;
    player->phone_num = rand() % 2;
}

void calculate_damage(PLAYER *target_player, int damage) {
    target_player->hp -= damage;
    if (target_player->hp < 0) {
        target_player->hp = 0;
    }
}

void use_item(PLAYER *player, const char *item, int *bullets) {
    if (strcmp(item, "knife") == 0 && player->knife_num) {
        player->knife_num = 0; // 使用后失效
    } else if (strcmp(item, "beer") == 0 && player->beer_num) {
        player->beer_num = 0;
        player->hp += 2; // 恢复2点血量
    } else if (strcmp(item, "phone") == 0 && player->phone_num) {
        player->phone_num = 0;
        // 随机显示一个子弹是否为空
        int revealIndex = rand() % 5;
        printf("Phone reveals: Bullet %d is %s\n", revealIndex + 1, bullets[revealIndex] ? "Real" : "Empty");
    }
}