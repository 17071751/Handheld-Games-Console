#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

uint16_t read_pixel_colour(uint16_t x, uint16_t y);

bool is_ui_pixel(uint16_t x, uint16_t y);

uint16_t read_ui_pixel(uint16_t x, uint16_t y);

// Pixel colour 1 is transparent
uint16_t read_sprite_pixel(uint16_t x, uint16_t y);

uint16_t read_background_pixel(uint16_t x, uint16_t y);

#endif
