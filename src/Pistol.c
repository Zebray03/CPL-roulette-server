#include "../include/Pistol.h"

#include <stdlib.h>

void initialize_pistol(PISTOL* pistol) {
    for (int i = 0; i < 5; i++) {
        pistol->is_real_bullet[i] = false;
        pistol->real_bullet_num = 0;
    }
}

void generate_bullets(PISTOL* pistol) {
    for (int i = 0; i < 5; i++) {
        pistol->is_real_bullet[i] = rand() % 2; // 0=空弹，1=实弹
        if (pistol->is_real_bullet[i] == 1) {
            pistol->real_bullet_num++;
        }
    }
}
