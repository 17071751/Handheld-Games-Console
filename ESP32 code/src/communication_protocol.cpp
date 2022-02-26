#include "communication_protocol.h"
#include "tilemap.h"
#include <stdint.h>

uint8_t read_data() {
    //uint8_t data = (uint8_t)REG_READ(GPIO_IN1_REG);

    //return (data & B111) | ((data >> 1) & B1000);

    return (digitalRead(36) << 3) | (digitalRead(34) << 2) | (digitalRead(33) << 1) | digitalRead(32);
}

void CommunicationProtocol::read_instruction() {
    switch (this->data) {
        case B0001: {
            this->active_instruction = ActiveInstruction::SetTilePostition;
            this->active_instruction_data.set_tile_postion_data = SetTilePositionData();
            break;
        }
        default: {
            this->active_instruction = ActiveInstruction::Waiting;
            break;
        }
    }
}

void CommunicationProtocol::process_set_tile_position() {
    SetTilePositionData* set_tile_data = &this->active_instruction_data.set_tile_postion_data;
    switch (set_tile_data->stage) {
        // Stage 0: choose map
        case 0: set_tile_data->ui_map = this->data; ++set_tile_data->stage; break;
        Serial.printf("ui map state: %d\n", set_tile_data->ui_map);

        // Stage 1: choose first 4 bits of x
        case 1: set_tile_data->x = this->data; ++set_tile_data->stage; break;
        // Stage 2: choose second 4 bits of x
        case 2: set_tile_data->x |= this->data << 4; ++set_tile_data->stage; break;
        Serial.printf("x state: %d\n", set_tile_data->x);

        // Stage 3: choose first 4 bits of y
        case 3: set_tile_data->y = this->data; ++set_tile_data->stage; break;
        // Stage 4: choose second 4 bits of y
        case 4: set_tile_data->y |= this->data << 4; ++set_tile_data->stage; break;
        Serial.printf("y state: %d\n", set_tile_data->y);

        // Stage 5: choose first 4 bits of tile ID
        case 5: set_tile_data->tile_id = this->data; ++set_tile_data->stage; break;
        // Stage 6: choose second 4 bits of tile ID
        case 6: set_tile_data->tile_id |= this->data << 4; ++set_tile_data->stage; break;
        // Stage 7: choose third 4 bits of tile ID
        case 7: set_tile_data->tile_id |= (uint16_t)(this->data) << 8; ++set_tile_data->stage; break;
        // Stage 8: choose forth 4 bits of tile ID and set chosen value
        case 8:
            set_tile_data->tile_id |= (uint16_t)(this->data) << 12;
            Serial.printf("tile_id state: %d\n", set_tile_data->tile_id);
            if (set_tile_data->ui_map) {
                // Verify that position is valid
                if (set_tile_data->x >= 40 || set_tile_data->y >= 30) {
                    Serial.printf("USER ERROR: process_set_tile_position: UI tile out of range (x: %d, y: %d)\n",
                        set_tile_data->x, set_tile_data->y);
                    this->active_instruction = ActiveInstruction::Waiting; break;
                }

                // Set tile id on UI tile map
                TileMapUI::tileMap->tiles[set_tile_data->y * 40 + set_tile_data->x] = set_tile_data->tile_id;
                Serial.printf("Set tile at x: %d, y: %d to id: %d\n", set_tile_data->x, set_tile_data->y, set_tile_data->tile_id);
            } else {
                // Verify that position is valid
                if (set_tile_data->x >= 45 || set_tile_data-> y >= 35) {
                    Serial.printf("USER ERROR: process_set_tile_position: Main tile out of range (x: %d, y: %d)\n",
                        set_tile_data->x, set_tile_data->y);
                    this->active_instruction = ActiveInstruction::Waiting; break;
                }

                // Set tile id on UI tile map
                TileMapMain::tileMap->tiles[set_tile_data->y * 45 + set_tile_data->x] = set_tile_data->tile_id;
                Serial.printf("Set tile at x: %d, y: %d to id: %d\n", set_tile_data->x, set_tile_data->y, set_tile_data->tile_id);
            }
            // Reset instuction to wait for next instruction
            this->active_instruction = ActiveInstruction::Waiting; break;
        default:
            Serial.printf("BUG: process_set_tile_position: Unreachable stage: %d\n",
                this->active_instruction_data.set_tile_postion_data.stage);
            break;
    }
}

void CommunicationProtocol::process_instruction() {
    this->data = read_data();
    switch (this->active_instruction) {
        case ActiveInstruction::Waiting: read_instruction(); break;
        case ActiveInstruction::SetTilePostition: process_set_tile_position(); break;
    }
    this->acknowledge_state = !this->acknowledge_state;
    Serial.printf("Acknowledge %d%d%d%d\n", ((this->data >> 3) & 1),
     ((this->data >> 2) & 1), ((this->data >> 1) & 1), (this->data & 1));
    digitalWrite(ACKNOWLEDGE_PIN, this->acknowledge_state);
}


