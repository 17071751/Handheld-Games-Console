#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

struct Sprite {
    uint16_t pixels[64];
    int16_t x; // X of 0xFFFF means that the sprite is disabled
    int16_t y;

    static constexpr Sprite* sprites = (Sprite*)1073577374; // 0x3FFD7D9E;

    static void initialise() {
        for (int i = 0; i < 64; ++i) {
            sprites[i].x = 0x0000;
            sprites[i].y = 0x0000;
            for (int j = 0; j < 64; ++j) {
                sprites[i].pixels[j] = 0;
            }
        }
    }
};

#endif


