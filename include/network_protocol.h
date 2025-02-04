#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#define DEFAULT_PORT 8080

typedef enum {
    ACKNOWLEDGE,
    JOIN_REQUEST,
    GAME_BEGIN,
    GAME_OVER,
    GAME_STATE,
    ITEM_ASSIGN,
    ITEM_USE,
    PHONE_RESULT,
    PLAYER_SHOOT,
    PLAYER_STATE,
    SHOOT_RESULT,
} NetMessageType;

const char* type_to_string(NetMessageType type);

NetMessageType string_to_type(const char type_str[]);

#endif //NETWORK_PROTOCOL_H
