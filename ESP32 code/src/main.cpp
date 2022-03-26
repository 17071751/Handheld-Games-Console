#include <Arduino.h>
#include <tilemap.h>
#include "communication_protocol.h"
#include "screen.h"

CommunicationProtocol communication_protocol = CommunicationProtocol();

void clock_interrupt() {
    communication_protocol.process_instruction();
}

void setup() {
  Serial.begin(9600);
  delay(100);

  Serial.println("\nInitialising");
  Tile::initialise();
  TileMapMain::initilise();
  TileMapUI::initilise();
  Sprite::initialise();

  pinMode(DATA0_PIN, INPUT);
  pinMode(DATA1_PIN, INPUT);
  pinMode(DATA2_PIN, INPUT);
  pinMode(DATA3_PIN, INPUT);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(ACKNOWLEDGE_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_interrupt, RISING);

  /*uint16_t tile[64] = {
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E0,
    0x1F, 0x00, 0xF800, 0xF800, 0xF800, 0xF800, 0x00, 0x7E0,
    0x1F, 0x00, 0xF800, 0x00, 0x00, 0xF800, 0x00, 0x7E0,
    0x1F, 0x00, 0xF800, 0x00, 0x00, 0xF800, 0x00, 0x7E0,
    0x1F, 0x00, 0xF800, 0xF800, 0xF800, 0xF800, 0x00, 0x7E0,
    0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E0,
    0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0,
  };

  for (int i = 0; i < 64; ++i) {
    Tile::tiles[0].pixels[i] = tile[i];
  };*/

  screen_setup();
}

void loop() {
  draw();
}