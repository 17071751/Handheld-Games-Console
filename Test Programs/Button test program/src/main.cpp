#include <Arduino.h>

void setup() {
  pinMode(19, INPUT_PULLDOWN);
  pinMode(18, INPUT_PULLDOWN);
  pinMode(14, INPUT_PULLDOWN);
  pinMode(15, INPUT_PULLDOWN);
  pinMode(40, INPUT_PULLDOWN);
  pinMode(41, INPUT_PULLDOWN);
  pinMode(17, INPUT_PULLDOWN);
  pinMode(16, INPUT_PULLDOWN);
}

void loop() {
  uint8_t button_status = (uint8_t)(GPIO6_PSR >> 16);
  Serial.printf("Up: %d, Down: %d, Left: %d, Right: %d, A: %d, B: %d, X: %d, Y: %d\n",
    button_status & 1,
    (button_status >> 1) & 1,
    (button_status >> 2) & 1,
    (button_status >> 3) & 1,
    (button_status >> 4) & 1,
    (button_status >> 5) & 1,
    (button_status >> 6) & 1,
    (button_status >> 7) & 1);
  
  delay(1500);
}