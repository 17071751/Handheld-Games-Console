#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <Arduino.h>
#include "tile.h"
#include "sprite.h"

#define DATA0_PIN 32
#define DATA1_PIN 33
#define DATA2_PIN 34
#define DATA3_PIN 36
#define CLOCK_PIN 5
#define ACKNOWLEDGE_PIN 4


enum class ActiveInstruction {
    Waiting, SetTilePostition, SetTileData, SetSpriteData, SetSpritePosition
};


struct SetTilePositionData {
    uint16_t tile_id;
    uint8_t x;
    uint8_t y;
    uint8_t stage = 0;
    bool ui_map;
};

struct SetTileDataData {
    uint16_t tile_id;
    uint16_t stage = 0;
    Tile tile;
};

struct SetSpriteDataData {
    uint8_t sprite_id;
    uint16_t stage = 0;
    uint16_t pixels[64];
};

struct SetSpritePositionData {
    uint8_t sprite_id;
    uint8_t stage = 0;
    int16_t x;
    int16_t y;
};

union ActiveInstructionData {
    SetTilePositionData set_tile_postion_data;
    SetTileDataData set_tile_data_data;
    SetSpriteDataData set_sprite_data_data;
    SetSpritePositionData set_sprite_position_data;
    ActiveInstructionData() {}
};

class CommunicationProtocol {
    uint8_t data;
    ActiveInstruction active_instruction;
    ActiveInstructionData active_instruction_data;

    void read_instruction();
    void process_set_tile_position();
    void process_set_tile_data();
    void process_set_sprite_data();
    void process_set_sprite_position();

public:
    CommunicationProtocol() : active_instruction(ActiveInstruction::Waiting) {}
    void process_instruction();
};


#endif
