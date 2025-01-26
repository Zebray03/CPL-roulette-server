#include "../include/pistol.h"

#include <stdlib.h>

void initialize_pistol(Pistol* pistol) {
    pistol->bullet_remaining_num = ROUND_NUM;
    for (int i = 0; i < ROUND_NUM; i++) {
        pistol->is_real_bullet[i] = false;
    }
    pistol->real_bullet_num = 0;
    generate_bullets(pistol);
}

void generate_bullets(Pistol* pistol) {
    for (int i = 0; i < ROUND_NUM; i++) {
        pistol->is_real_bullet[i] = rand() % 2; // 0 = 空弹，1 = 实弹
        if (pistol->is_real_bullet[i] == 1) {
            pistol->real_bullet_num++;
        }
    }
}
