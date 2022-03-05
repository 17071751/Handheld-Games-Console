#ifndef TILE_H
#define TILE_H

#include <stdint.h>

struct Tile {
    uint16_t pixels[64];
    void clearTile();
    static void initialise();

    static constexpr Tile* tiles = (Tile*)1073588224; // 0x3FFDA800;
};

#endif