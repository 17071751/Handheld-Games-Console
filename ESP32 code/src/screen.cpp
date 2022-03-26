
#include "screen.h"
#include "tilemap.h"
#include "tile.h"
#include "sprite.h"
#include "Arduino.h"


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

void write_screen_data(uint8_t data) {
    digitalWrite(12, data & 1);
    digitalWrite(13, (data >> 1) & 1);
    digitalWrite(14, (data >> 2) & 1);
    digitalWrite(15, (data >> 3) & 1);
    digitalWrite(16, (data >> 4) & 1);
    digitalWrite(17, (data >> 5) & 1);
    digitalWrite(18, (data >> 6) & 1);
    digitalWrite(19, (data >> 7) & 1);
}

void command(uint8_t command) {
    digitalWrite(READ_SCREEN_DATA_PIN, HIGH);
    digitalWrite(SCREEN_DATA_COMMAND_SELECT_PIN, LOW);
    write_screen_data(command);
    digitalWrite(WRITE_SCREEN_DATA_PIN, LOW);
    // Set to low to indicate command
    // Confirm write
    digitalWrite(WRITE_SCREEN_DATA_PIN, HIGH);
    // Reset
    digitalWrite(SCREEN_DATA_COMMAND_SELECT_PIN, HIGH);
}

void write_data(uint8_t data) {
    write_screen_data(data);
    digitalWrite(WRITE_SCREEN_DATA_PIN, LOW);
    digitalWrite(WRITE_SCREEN_DATA_PIN, HIGH);
    digitalWrite(SCREEN_DATA_COMMAND_SELECT_PIN, HIGH);
}

void screen_setup() {
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(RESET_SCREEN_PIN, OUTPUT);
    pinMode(SCREEN_DATA_COMMAND_SELECT_PIN, OUTPUT);
    pinMode(WRITE_SCREEN_DATA_PIN, OUTPUT);
    pinMode(READ_SCREEN_DATA_PIN, OUTPUT);

    //write_screen_data(0xFF);
    digitalWrite(12, 1);
    digitalWrite(13, 1);
    digitalWrite(14, 1);
    digitalWrite(15, 1);
    digitalWrite(16, 1);
    digitalWrite(17, 1);
    digitalWrite(18, 1);
    digitalWrite(19, 1);
    Serial.println("Test");

    /*command(0x2A);
    write_data(0);
    write_data(1);// x
    write_data(319 >> 8);
    write_data(319 & 0xFF);// x

    command(0x2B);
    write_data(0);
    write_data(1);// y
    write_data(239 >> 8);
    write_data(239 & 0xFF);// y*/
}

void draw() {
    
    command(0x2C);

    // Write pixels
    Serial.println("Write pixels");
    for (int y = 0; y < 240; ++y) {
        for (int x = 0; x < 320; ++x) {
            write_data(B11111000);
            write_data(0);
        }
    }
    command(0);
    Serial.println("Pixels written");

}
