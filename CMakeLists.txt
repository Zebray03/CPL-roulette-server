cmake_minimum_required(VERSION 3.30)
project(CPL_roulette_server C)

set(CMAKE_C_STANDARD 11)

set(CJSON_SOURCES
        lib/cJSON/cJSON.c
)

set(CJSON_HEADERS
        lib/cJSON/cJSON.h
)

add_executable(CPL_roulette_server
        src/main.c
        src/player.c
        src/game.c
        src/server.c
        src/pistol.c
        src/network_protocol.c

        include/player.h
        include/game.h
        include/server.h
        include/pistol.h
        include/network_protocol.h

        ${CJSON_SOURCES}
        ${CJSON_HEADERS}
        src/network_controller.c
        include/network_controller.h
)

target_include_directories(CPL_roulette_server PRIVATE
        include
        lib/cJSON
)

link_libraries(ws2_32)
target_link_libraries(CPL_roulette_server ws2_32)
