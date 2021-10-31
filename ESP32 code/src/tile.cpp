#include "tile.h"
#include <stdlib.h>

void Tile::clearTile() {
    for (uintptr_t i = 0; i < 64; i++) {
        this->pixels[i] = 0;
    }
}

void Tile::initialise() {
    for (uintptr_t i = 0; i < 1200; i++) {
        Tile::tiles[i].clearTile();
    }
}