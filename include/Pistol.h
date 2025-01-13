#ifndef PISTOL_H
#define PISTOL_H

#include <stdbool.h>

struct Pistol {
    bool is_real_bullet[5];
    int real_bullet_num;
};

typedef struct Pistol PISTOL;

void initialize_pistol();

// 生成随机子弹
void generate_bullets(PISTOL *pistol);

#endif //PISTOL_H
