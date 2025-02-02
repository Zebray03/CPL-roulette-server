#include "../include/game.h"

#include <math.h>
#include <stdio.h>

void initialize_game(Game* game) {
    initialize_player(game->demon);
    initialize_player(game->gambler);
    initialize_pistol(game->pistol);
    game->demon_turn = true; // 默认恶魔先手
}

int shoot(Game* game, Player* source, Player* target) {
    bool is_real = game->pistol->is_real_bullet[ROUND_NUM - game->pistol->bullet_remaining_num];
    game->pistol->bullet_remaining_num--;

    int damage = 0;
    if (is_real) {
        if (source->knife_use) {
            damage = 2;
        }
        else {
            damage = 1;
        }
        calculate_damage(target, damage);
        game->demon_turn = !game->demon_turn;
        game->round++;
    }

    return damage;
}

bool is_game_over(Game* game) {
    return game->gambler->hp <= 0 || game->demon->hp <= 0;
}
