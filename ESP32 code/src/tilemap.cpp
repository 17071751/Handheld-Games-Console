#include "tilemap.h"
#include <Arduino.h>

void TileMapMain::initilise() {
    Serial.printf("Tilemap main initialising at: 0x%08X", (uintptr_t)TileMapMain::tileMap);
    for (uintptr_t i = 0; i < 35*45; i++) {
        TileMapMain::tileMap->tiles[i] = 0;
    }
}

void TileMapUI::initilise() {
    Serial.printf("Tilemap UI initialising at: 0x%08X", (uintptr_t)TileMapUI::tileMap);
    for (uintptr_t i = 0; i < 30*40; i++) {
        TileMapUI::tileMap->tiles[i] = 0;
    }
}