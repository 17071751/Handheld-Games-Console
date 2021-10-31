#include "tile.h"

struct TileMapMain {
    uint32_t tiles[35*45];
    static constexpr TileMapMain* tileMap = (TileMapMain*)1073582674; // 0x3FFD_9252;
    static const uint8_t WIDTH = 45;
    static const uint8_t HEIGHT = 35;
    
    static void initilise();
};

struct TileMapUI {
    uint32_t tiles[30*40];
    static constexpr TileMapUI* tileMap = (TileMapUI*)1073585824; // 0x3FFD_9EA0;
    static const uint8_t WIDTH = 40;
    static const uint8_t HEIGHT = 30;

    static void initilise();
};
