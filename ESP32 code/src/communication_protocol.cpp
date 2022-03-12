#include "communication_protocol.h"
#include "tilemap.h"
#include <stdint.h>

uint8_t acknowledge_state = 0;

uint8_t read_data() {
    //uint8_t data = (uint8_t)REG_READ(GPIO_IN1_REG);

    //return (data & B111) | ((data >> 1) & B1000);
    uint8_t val = (digitalRead(36) << 3) | (digitalRead(34) << 2) | (digitalRead(33) << 1) | digitalRead(32);

    return val;
}

void CommunicationProtocol::read_instruction() {
    switch (this->data) {
        case B0001: {
            this->active_instruction = ActiveInstruction::SetTilePostition;
            this->active_instruction_data.set_tile_postion_data = SetTilePositionData();
            break;
        }
        case B0010: {
            this->active_instruction = ActiveInstruction::SetTileData;
            this->active_instruction_data.set_tile_data_data = SetTileDataData();
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

void CommunicationProtocol::process_set_tile_data() {
    SetTileDataData* set_tile_data = &this->active_instruction_data.set_tile_data_data;
    switch (set_tile_data->stage) {
        // Stage 0: choose first 4 bits of tile ID
        case 0: set_tile_data->tile_id = this->data; ++set_tile_data->stage; Serial.println("id0"); break;
        // Stage 1: choose second 4 bits of tile ID
        case 1: set_tile_data->tile_id |= this->data << 4; ++set_tile_data->stage; Serial.println("id1"); break;
        // Stage 2: choose third 4 bits of tile ID
        case 2: set_tile_data->tile_id |= this->data << 8; ++set_tile_data->stage; Serial.println("id2"); break;
        // Stage 3: choose forth 4 bits of tile ID
        case 3: set_tile_data->tile_id |= this->data << 12; ++set_tile_data->stage; Serial.println("id3"); break;
        // Stage 4-69: choose pixels
        default: {
            int i = set_tile_data->stage - 4;
            int current_pixel = i/4;
            int pixel_stage = i%4;
            Serial.printf("current_pixel: %d, pixel_stage: %d\n", current_pixel, pixel_stage);
            switch (pixel_stage) {
                // Stage i*4: choose first 4 bits of pixel
                case 0: set_tile_data->tile.pixels[current_pixel] = this->data; ++set_tile_data->stage; break;
                // Stage i*4+1: choose second 4 bits of pixel
                case 1: set_tile_data->tile.pixels[current_pixel] |= this->data << 4; ++set_tile_data->stage; break;
                // Stage i*4+2: choose third 4 bits of pixel
                case 2: set_tile_data->tile.pixels[current_pixel] |= this->data << 8; ++set_tile_data->stage; break;
                // Stage i*4+3: choose forth 4 bits of pixel
                case 3: set_tile_data->tile.pixels[current_pixel] |= this->data << 12; ++set_tile_data->stage; break;
            }
            // If all pixels read set tile
            if (current_pixel == 63 && pixel_stage == 3) {
                Tile::tiles[set_tile_data->tile_id] = set_tile_data->tile;

                Serial.printf("Tile ID: %d\nTile data: {\n", set_tile_data->tile_id);
                for (int y = 5; y < 8; ++y) { // Cannot print all values as the interrupt will crash due to taking too long
                    for (int x = 0; x < 8; ++x) {
                        Serial.printf("%d, ", Tile::tiles[set_tile_data->tile_id].pixels[y*8+x]);
                        //Serial.printf("0, ");
                    }
                    Serial.println();
                }
                Serial.println("}");
                
                this->active_instruction = ActiveInstruction::Waiting;
            }
            break;
        }
    }
}

char* active_instruction_debug(ActiveInstruction active_instruction) {
    switch (active_instruction) {
        case ActiveInstruction::Waiting: return "Waiting";
        case ActiveInstruction::SetTilePostition: return "SetTilePostition";
        case ActiveInstruction::SetTileData: return "SetTileData";
    }
}

void CommunicationProtocol::process_instruction() {
    this->data = read_data();
    Serial.printf("Recieved %d%d%d%d Active instruction:%s\n", ((this->data >> 3) & 1),
     ((this->data >> 2) & 1), ((this->data >> 1) & 1), (this->data & 1), active_instruction_debug(this->active_instruction));

    switch (this->active_instruction) {
        case ActiveInstruction::Waiting: read_instruction(); break;
        case ActiveInstruction::SetTilePostition: process_set_tile_position(); break;
        case ActiveInstruction::SetTileData: process_set_tile_data(); break;
    }
    acknowledge_state = 1 - acknowledge_state;
    digitalWrite(ACKNOWLEDGE_PIN, acknowledge_state);
}


