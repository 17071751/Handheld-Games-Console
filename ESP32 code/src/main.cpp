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
  while(!Serial) {}

  Serial.println("\nInitialising");
  /*Tile::initialise();
  TileMapMain::initilise();
  TileMapUI::initilise();

  pinMode(DATA0_PIN, INPUT);
  pinMode(DATA1_PIN, INPUT);
  pinMode(DATA2_PIN, INPUT);
  pinMode(DATA3_PIN, INPUT);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(ACKNOWLEDGE_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_interrupt, RISING);

  screen_setup();
  //draw();*/
  pinMode(13, OUTPUT);
  Serial.println("Setting Pin");
  digitalWrite(13, HIGH);
  Serial.println("Pin, Set");
}

void loop() {

}