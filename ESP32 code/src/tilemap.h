#ifndef TILEMAP_H
#define TILEMAP_H

#include "tile.h"

struct TileMapMain {
    static const uint8_t WIDTH = 45;
    static const uint8_t HEIGHT = 35;
    uint32_t tiles[WIDTH*HEIGHT];
    static constexpr TileMapMain* tileMap = (TileMapMain*)1073582674; // 0x3FFD_9252;
    
    static void initilise();
};

struct TileMapUI {
    static const uint8_t WIDTH = 40;
    static const uint8_t HEIGHT = 30;
    uint32_t tiles[WIDTH*HEIGHT];
    static constexpr TileMapUI* tileMap = (TileMapUI*)1073585824; // 0x3FFD_9EA0;

    static void initilise();
};

#endif