#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

struct Sprite {
    uint16_t pixels[64];
    uint16_t x; // X of 0xFFFF means that the sprite is disabled
    uint16_t y;

    static constexpr Sprite* sprites = (Sprite*)1073577374; // 0x3FFD7D9E;
};

#endif


