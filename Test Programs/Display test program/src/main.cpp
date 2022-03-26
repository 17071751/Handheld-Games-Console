#include <Arduino.h>


#define CP_DATA_0 2
#define CP_DATA_1 3
#define CP_DATA_2 4
#define CP_DATA_3 33
#define CP_CLOCK 5
#define CP_ACKNOWLEDGE 29

bool acknowledge = false;

uint16_t tile[64] = {
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E0,
  0x1F, 0x00, 0xF800, 0xF800, 0xF800, 0xF800, 0x00, 0x7E0,
  0x1F, 0x00, 0xF800, 0x00, 0x00, 0xF800, 0x00, 0x7E0,
  0x1F, 0x00, 0xF800, 0x00, 0x00, 0xF800, 0x00, 0x7E0,
  0x1F, 0x00, 0xF800, 0xF800, 0xF800, 0xF800, 0x00, 0x7E0,
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E0,
  0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0, 0x7E0,
};

void writeMessage(uint8_t message) {
  if (message & 0xF0) {
    Serial.printf("Message %d bigger than 4 bits\n", message);
  }
  acknowledge = false;

  digitalWrite(CP_DATA_0, message & 1);
  digitalWrite(CP_DATA_1, (message >> 1) & 1);
  digitalWrite(CP_DATA_2, (message >> 2) & 1);
  digitalWrite(CP_DATA_3, (message >> 3) & 1);
  digitalWrite(CP_CLOCK, 1);
  digitalWrite(CP_CLOCK, 0);
  while (!acknowledge) {delay(1);}
}

void acknowledged() {
  Serial.println("Messaged acknowledged and processed");
  acknowledge = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("Serial test");
  // put your setup code here, to run once:
  delay(100); // Give the other microcontroller plenty of time to setup

  pinMode(CP_DATA_0, OUTPUT);
  pinMode(CP_DATA_1, OUTPUT);
  pinMode(CP_DATA_2, OUTPUT);
  pinMode(CP_DATA_3, OUTPUT);
  pinMode(CP_CLOCK, OUTPUT);
  pinMode(CP_ACKNOWLEDGE, INPUT);
  attachInterrupt(digitalPinToInterrupt(CP_ACKNOWLEDGE), acknowledged, CHANGE);


  // Set tile
  writeMessage(2);
  // Write tile ID of 0
  writeMessage(0);
  writeMessage(0);
  writeMessage(0);
  writeMessage(0);

  // Write pixels
  for (int i = 0; i < 64; ++i) {
    // Write pixel
    writeMessage(tile[i] & 0xF);
    writeMessage((tile[i] >> 4) & 0xF);
    writeMessage((tile[i] >> 8) & 0xF);
    writeMessage((tile[i] >> 12) & 0xF);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}