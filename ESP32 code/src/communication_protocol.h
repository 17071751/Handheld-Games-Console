#include <Arduino.h>

#define DATA0_PIN 32
#define DATA1_PIN 33
#define DATA2_PIN 34
#define DATA3_PIN 36
#define CLOCK_PIN 5
#define ACKNOWLEDGE_PIN 4



enum class ActiveInstruction {
    Waiting, SetTilePostition
};

struct SetTilePositionData {
    uint16_t tile_id;
    uint8_t x;
    uint8_t y;
    uint8_t stage = 0;
    bool ui_map;
};

union ActiveInstructionData {
    int none;
    SetTilePositionData set_tile_postion_data;
    ActiveInstructionData() {}
};

class CommunicationProtocol {
    ActiveInstruction active_instruction;
    ActiveInstructionData active_instruction_data;
    bool acknowledge_state;
    uint8_t data;

    void read_instruction();
    void process_set_tile_position();

public:
    CommunicationProtocol() : active_instruction(ActiveInstruction::Waiting), acknowledge_state(LOW) {}
    void process_instruction();
};
