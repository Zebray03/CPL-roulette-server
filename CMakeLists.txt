cmake_minimum_required(VERSION 3.30)
project(CPL_roulette_server C)

set(CMAKE_C_STANDARD 11)

add_executable(CPL_roulette_server
        src/main.c
        src/Player.c
        include/Player.h
        src/Game.c
        include/Game.h
        src/Server.c
        include/Server.h
        src/Pistol.c
        include/Pistol.h)

link_libraries(ws2_32)
target_link_libraries(CPL_roulette_server ws2_32)
