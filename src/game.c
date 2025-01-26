#include "../include/game.h"
#include "../util/socket.h"

#include <stdio.h>

void initialize_game(Game* game) {
    initialize_player(&game->gambler);
    initialize_player(&game->demon);
    initialize_pistol(&game->pistol);
    game->gambler_turn = false;
}

void shoot(Game* game, Player* source, Player* target,
           SOCKET current_socket, char current_buffer[],
           SOCKET opponent_socket, char opponent_buffer[]) {
    bool is_real = game->pistol.is_real_bullet[ROUND_NUM - game->pistol.bullet_remaining_num];
    game->pistol.bullet_remaining_num--;

    if (is_real) {
        if (source->knife_use) {
            calculate_damage(target, 2);
        }
        else {
            calculate_damage(target, 1);
        }
        game->gambler_turn = !game->gambler_turn;
    }
    source->knife_use = false;
    source->beer_use = false;
    source->phone_use = false;

    if (is_real) {
        sprintf(current_buffer, "Bullet %d was real. Exchange.\n",
                ROUND_NUM - game->pistol.bullet_remaining_num);
        sprintf(opponent_buffer, "Bullet %d was real. Exchange.\n",
                ROUND_NUM - game->pistol.bullet_remaining_num);
    }
    else {
        sprintf(current_buffer, "Bullet %d was empty. %s continues.\n",
                ROUND_NUM - game->pistol.bullet_remaining_num, game->gambler_turn == 1 ? "Gambler" : "Demon");
        sprintf(opponent_buffer, "Bullet %d was empty. %s continues.\n",
                ROUND_NUM - game->pistol.bullet_remaining_num, game->gambler_turn == 1 ? "Gambler" : "Demon");
    }
    send_message(current_socket, current_buffer);
    send_message(opponent_socket, opponent_buffer);
}

bool is_game_over(Game* game) {
    return game->gambler.hp <= 0 || game->demon.hp <= 0;
}
