#include <stdint.h>

class Tile {
    uint16_t pixels[64];

public:
    void clearTile();
    static void initialise();

    static constexpr Tile* tiles = (Tile*)1073588224; // 0x3FFDA800;
};