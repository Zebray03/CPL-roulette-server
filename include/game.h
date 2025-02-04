#ifndef GAME_H
#define GAME_H

#include "player.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

typedef struct game {
    Player demon;
    Player gambler;
    Pistol pistol;
    bool demon_turn;
    int round;
} Game;

void start_pvp_game(Game* game, SOCKET demon_socket, SOCKET gambler_socket);

#endif //GAME_H
