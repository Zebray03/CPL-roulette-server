#include "../include/pistol.h"

#include <stdlib.h>
#include <time.h>

#define MIN_REAL_BULLETS 1
#define MAX_REAL_BULLETS 3

void initialize_pistol(Pistol* pistol) {
    pistol->bullet_remaining_num = ROUND_NUM;
    for (int i = 0; i < ROUND_NUM; i++) {
        pistol->is_real_bullet[i] = false;
    }
    pistol->real_bullet_num = 0;
}

void generate_bullets(Pistol* pistol) {
    srand(time(NULL));
    pistol->real_bullet_num = MIN_REAL_BULLETS + rand() % (MAX_REAL_BULLETS - MIN_REAL_BULLETS + 1);
    for (int i = 0; i < pistol->real_bullet_num; i++) {
        int pos;
        do {
            pos = rand() % ROUND_NUM;
        }
        while (pistol->is_real_bullet[pos]);
        pistol->is_real_bullet[pos] = true;
    }
}
