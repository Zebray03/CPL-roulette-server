#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Pistol.h"

struct Game {
    PLAYER gambler;
    PLAYER demon;
    PISTOL pistol;
    bool gambler_turn;
};

typedef struct Game GAME;

void initialize_game(GAME *game);

// 游戏回合逻辑
void handle_turn(GAME *game, int bulletIndex);

bool is_game_over(GAME *game);

#endif //GAME_H
