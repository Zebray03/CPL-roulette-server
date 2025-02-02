#ifndef GAME_H
#define GAME_H

#include "player.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

typedef struct game {
    Player* demon;
    Player* gambler;
    Pistol* pistol;
    bool demon_turn;
    int round;
} Game;

void initialize_game(Game* game);

int shoot(Game* game, Player* source, Player* dest);

bool is_game_over(Game* game);

#endif //GAME_H
