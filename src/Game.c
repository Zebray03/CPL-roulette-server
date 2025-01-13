#include "../include/Game.h"

void initialize_game(GAME *game) {
    initialize_player(&game->gambler);
    initialize_player(&game->demon);
    initialize_pistol(&game->pistol);
    game->gambler_turn = false;
}

void handle_turn(GAME *game, int bulletIndex) {

}

bool is_game_over(GAME *game) {
    return game->gambler.hp <= 0 || game->demon.hp <= 0;
}