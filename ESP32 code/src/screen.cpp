
#include "screen.h"
#include "tilemap.h"
#include "tile.h"
#include "sprite.h"


uint16_t read_pixel_colour(uint16_t x, uint16_t y) {
    
    if (is_ui_pixel(x, y)) {
        return read_ui_pixel(x,y);
    }

    uint16_t colour = read_sprite_pixel(x, y);
    if (colour != 1) {
        return colour;
    }

    return read_background_pixel(x, y);
}

bool is_ui_pixel(uint16_t x, uint16_t y) {
    uint8_t ui_x_pos = x / 8;
    uint8_t ui_y_pos = y / 8;

    return TileMapUI::tileMap->tiles[ui_y_pos * TileMapUI::WIDTH + ui_x_pos] != 0;
}

uint16_t read_ui_pixel(uint16_t x, uint16_t y) {
    uint8_t ui_x_pos = x / 8;
    uint8_t ui_y_pos = y / 8;
    uint8_t tile_x_pos = x % 8;
    uint8_t tile_y_pos = y % 8;

    uint32_t tile_id = TileMapUI::tileMap->tiles[ui_y_pos * TileMapUI::WIDTH + ui_x_pos];

    uint16_t pixel = Tile::tiles[tile_id].pixels[tile_y_pos * 8 + tile_x_pos];

    return pixel;
}

// Pixel colour 1 is transparent
uint16_t read_sprite_pixel(uint16_t x, uint16_t y) {
    for (int sprite_index = 0; sprite_index < 64; ++sprite_index) {
        Sprite* sprite = &Sprite::sprites[sprite_index];
        int16_t sprite_x_pos = sprite->x;
        int16_t sprite_y_pos = sprite->y;

        int16_t sprite_local_x_pos = sprite_x_pos - x;
        int16_t sprite_local_y_pos = sprite_y_pos - y;

        if (sprite_local_y_pos < 0 || sprite_local_x_pos < 0 || sprite_local_y_pos >= 8 || sprite_local_x_pos >= 8) {
            continue;
        }

        return sprite->pixels[sprite_local_y_pos * 8 + sprite_local_x_pos];
    }
    return 1;
}

uint16_t read_background_pixel(uint16_t x, uint16_t y) {
    uint8_t background_x_pos = x / 8;
    uint8_t background_y_pos = y / 8;
    uint8_t tile_x_pos = x % 8;
    uint8_t tile_y_pos = y % 8;

    uint32_t tile_id = TileMapMain::tileMap->tiles[background_x_pos * TileMapUI::WIDTH + background_y_pos];

    uint16_t pixel = Tile::tiles[tile_id].pixels[tile_y_pos * 8 + tile_x_pos];

    return pixel;
}