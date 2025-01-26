#ifndef GAME_H
#define GAME_H

#include "player.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

typedef struct game {
    Player gambler;
    Player demon;
    Pistol pistol;
    bool gambler_turn;
} Game;

void initialize_game(Game* game);

void shoot(Game* game, Player* source, Player* dest,
           SOCKET current_socket, char current_buffer[],
           SOCKET opponent_socket, char opponent_buffer[]);

bool is_game_over(Game* game);

#endif //GAME_H
