#include <Arduino.h>
#include <tilemap.h>
#include "communication_protocol.h"

CommunicationProtocol communication_protocol = CommunicationProtocol();

void clock_interrupt() {
    communication_protocol.process_instruction();
}

void setup() {
  Tile::initialise();
  TileMapMain::initilise();
  TileMapUI::initilise();

  pinMode(DATA0_PIN, INPUT);
  pinMode(DATA1_PIN, INPUT);
  pinMode(DATA2_PIN, INPUT);
  pinMode(DATA3_PIN, INPUT);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(ACKNOWLEDGE_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_interrupt, RISING);
}

void loop() {

}