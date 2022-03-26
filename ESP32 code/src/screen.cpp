
#include "screen.h"
#include "tilemap.h"
#include "tile.h"
#include "sprite.h"
#include "Arduino.h"


uint16_t read_pixel_colour(uint16_t x, uint16_t y) {
    
    /*if (is_ui_pixel(x, y)) {
        read_ui_pixel(x,y);
        return read_ui_pixel(x,y);
    }*/

    /*uint16_t colour = read_sprite_pixel(x, y);
    if (colour != 1) {
        return colour;
    }*/

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

    Serial.printf("tile_x_pos: %d, tile_y_pos: %d, tile_id: %d, index: %d\n", tile_x_pos, tile_y_pos, tile_id, ui_y_pos * TileMapUI::WIDTH + ui_x_pos);

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

    uint32_t tile_id = TileMapMain::tileMap->tiles[background_y_pos * TileMapMain::WIDTH + background_x_pos];

    uint16_t pixel = Tile::tiles[tile_id].pixels[tile_y_pos * 8 + tile_x_pos];

    return pixel;
}

void write_screen_data(uint8_t data) {
    REG_WRITE(GPIO_OUT_W1TC_REG, 0xFF << 12);
    REG_WRITE(GPIO_OUT_W1TS_REG, data << 12);
}

void command(uint8_t command) {
    digitalWrite(WRITE_SCREEN_DATA_PIN, LOW);
    digitalWrite(SCREEN_DATA_COMMAND_SELECT_PIN, LOW);
    delay(5);
    write_screen_data(command);
    digitalWrite(WRITE_SCREEN_DATA_PIN, HIGH);
    digitalWrite(SCREEN_DATA_COMMAND_SELECT_PIN, HIGH);
    delay(5);
}

void write_data(uint8_t data) {
    write_screen_data(data);
    digitalWrite(WRITE_SCREEN_DATA_PIN, LOW);
    digitalWrite(WRITE_SCREEN_DATA_PIN, HIGH);
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
    digitalWrite(RESET_SCREEN_PIN, HIGH);
    digitalWrite(WRITE_SCREEN_DATA_PIN, HIGH);
    digitalWrite(READ_SCREEN_DATA_PIN, HIGH);
    digitalWrite(SCREEN_DATA_COMMAND_SELECT_PIN, HIGH);

    digitalWrite(RESET_SCREEN_PIN, LOW);
    delay(100);
    digitalWrite(RESET_SCREEN_PIN, HIGH);
    delay(100);

    // Set pixel format
    command(0x3A);
    write_data(0x55);

    // Exit sleep mode
    command(0x11);
    delay(10);

    // Display on
    command(0x29);
    delay(10);
    // Normal display mode
    command(0x13);

    command(0x2A); // Set column address
    write_data(0);
    write_data(0);
    write_data(0);
    write_data(0xEF);

    command(0x2B); // Set row address
    write_data(0);
    write_data(0);
    write_data(0x1);
    write_data(0x3F);

}

void draw() {
    //read_pixel_colour(72, 119);
    command(0x2C);

    // Write pixels
    for (int x = 0; x < 320; ++x) {//320; ++x) {
        for (int y = 0; y < 240; ++y) {//240; ++y) {
            uint16_t colour = read_pixel_colour(x, y);

            write_data(colour & 0xFF);
            write_data(colour >> 8);
            //write_data(0);
            //write_data(0);
        }
    }
    command(0);
    /*// Write pixels
    for (int x = 319; x >= 0; --x) {
        for (int y = 239; y >= 0; --y) {
            uint16_t colour = read_pixel_colour(x, y);

            write_data(colour & 0xFF);
            write_data(colour >> 8);
        }
    }
    command(0);*/
}
