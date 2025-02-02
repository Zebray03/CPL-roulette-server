#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#define DEFAULT_PORT 8080

typedef enum {
    JOIN_REQUEST,
    GAME_BEGIN,
    GAME_STATE,
    ITEM_ASSIGN,
    PLAYER_STATE,
    ITEM_USE,
    PLAYER_SHOOT,
    SHOOT_RESULT,
    GAME_OVER
} NetMessageType;

const char* type_to_string(NetMessageType type);

NetMessageType string_to_type(const char type_str[]);

#endif //NETWORK_PROTOCOL_H
