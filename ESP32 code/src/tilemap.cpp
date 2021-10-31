#include "tilemap.h"

void TileMapMain::initilise() {
    for (uintptr_t i = 0; i < 35*45; i++) {
        TileMapMain::tileMap->tiles[i] = 0;
    }
}

void TileMapUI::initilise() {
    for (uintptr_t i = 0; i < 30*40; i++) {
        TileMapUI::tileMap->tiles[i] = 0;
    }
}