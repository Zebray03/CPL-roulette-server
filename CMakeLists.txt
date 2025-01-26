cmake_minimum_required(VERSION 3.30)
project(CPL_roulette_server C)

set(CMAKE_C_STANDARD 11)

add_executable(CPL_roulette_server
        src/main.c
        src/player.c
        include/player.h
        src/game.c
        include/game.h
        src/server.c
        include/server.h
        src/pistol.c
        include/pistol.h
        util/socket.c
        util/socket.h
)

link_libraries(ws2_32)
target_link_libraries(CPL_roulette_server ws2_32)
