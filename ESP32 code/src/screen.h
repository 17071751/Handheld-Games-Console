#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define RESET_SCREEN_PIN 27
#define SCREEN_DATA_COMMAND_SELECT_PIN 21
#define WRITE_SCREEN_DATA_PIN 22
#define READ_SCREEN_DATA_PIN 23

uint16_t read_pixel_colour(uint16_t x, uint16_t y);

bool is_ui_pixel(uint16_t x, uint16_t y);

uint16_t read_ui_pixel(uint16_t x, uint16_t y);

// Pixel colour 1 is transparent
uint16_t read_sprite_pixel(uint16_t x, uint16_t y);

uint16_t read_background_pixel(uint16_t x, uint16_t y);

void screen_setup();

void draw(uint16_t colour);

void command(uint8_t command);

#endif
