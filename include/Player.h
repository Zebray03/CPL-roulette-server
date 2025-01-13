#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    int hp;
    int knife_num;
    int beer_num;
    int phone_num;
};

typedef struct Player PLAYER;

void initialize_player(PLAYER* player);

// 分配道具
void assign_items(PLAYER *player);

void calculate_damage(PLAYER* target_player, int damage);

// 处理道具使用逻辑
void use_item(PLAYER* player, const char* item, int* bullets);

#endif //PLAYER_H
