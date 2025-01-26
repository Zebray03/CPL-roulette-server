#ifndef PISTOL_H
#define PISTOL_H

#include <stdbool.h>

#define ROUND_NUM 5

typedef struct pistol {
    int bullet_remaining_num;
    bool is_real_bullet[ROUND_NUM];
    int real_bullet_num;
} Pistol;

void initialize_pistol(Pistol* pistol);

void generate_bullets(Pistol* pistol);

#endif //PISTOL_H
