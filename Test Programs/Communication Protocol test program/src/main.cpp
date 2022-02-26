#include <Arduino.h>

#define CP_DATA_0 2
#define CP_DATA_1 3
#define CP_DATA_2 4
#define CP_DATA_3 33
#define CP_CLOCK 5
#define CP_ACKNOWLEDGE 29

/*
The commands will be 4 bits in length
The commands will be:
0000 - No Command
0001 - Set Tile position
0010 - Read Tile position
0011 - Set Tile Data
0100 - Read Tile Data
0101 - Set Sprite Data
0110 - Read Sprite Data
0111 - Interact with Sprites on sprite layer
1000 - Reserved
1101 - Reserved
1110 - Reserved
1111 - Reserved
*/

bool acknowledge = false;

void writeMessage(uint8_t message) {
  if (message & 0xF0) {
    Serial.printf("Message %d bigger than 4 bits\n", message);
  }

  digitalWrite(CP_DATA_0, message & 1);
  digitalWrite(CP_DATA_1, (message >> 1) & 1);
  digitalWrite(CP_DATA_2, (message >> 2) & 1);
  digitalWrite(CP_DATA_3, (message >> 3) & 1);
  digitalWrite(CP_CLOCK, 1);
  digitalWrite(CP_CLOCK, 0);
}

void acknowledged() {
  Serial.println("Messaged acknowledged and processed");
  acknowledge = true;
}

void testSetTilePosition() {
  // select "Set Tile position"
  Serial.println("select \"Set Tile position\"");
  acknowledge = false;
  writeMessage(1);
  while (!acknowledge) {delay(1);}
  // Write to tile map
  Serial.println("Write to tile map");
  acknowledge = false;
  writeMessage(0);
  while (!acknowledge) {delay(1);}
  // Write x of 26
  Serial.println("Write x of 26 part 1");
  acknowledge = false;
  writeMessage(26 & 0xF);
  while (!acknowledge) {delay(1);}
  Serial.println("Write x of 26 part 2");
  acknowledge = false;
  writeMessage(26 >> 4);
  while (!acknowledge) {delay(1);}
  // Write y of 17
  Serial.println("Write y of 17 part 1");
  acknowledge = false;
  writeMessage(17 & 0xF);
  while (!acknowledge) {delay(1);}
  Serial.println("Write y of 17 part 2");
  acknowledge = false;
  writeMessage(17 >> 4);
  while (!acknowledge) {delay(1);}
  // Write tile ID of 36
  Serial.println("Write tile ID of 36 part 1");
  acknowledge = false;
  writeMessage(36 & 0xF);
  while (!acknowledge) {delay(1);}
  Serial.println("Write tile ID of 36 part 2");
  acknowledge = false;
  writeMessage(36 >> 4);
  while (!acknowledge) {delay(1);}
  Serial.println("Write tile ID of 36 part 3");
  acknowledge = false;
  writeMessage(0);
  while (!acknowledge) {delay(1);}
  Serial.println("Write tile ID of 36 part 4");
  acknowledge = false;
  writeMessage(0);
  while (!acknowledge) {delay(1);}
}

void setup() {

  delay(5000);
  Serial.begin(9600);
  while(!Serial) {}
  Serial.println("Serial test");
  // put your setup code here, to run once:
  delay(1); // Give the other microcontroller plenty of time to setup

  pinMode(CP_DATA_0, OUTPUT);
  pinMode(CP_DATA_1, OUTPUT);
  pinMode(CP_DATA_2, OUTPUT);
  pinMode(CP_DATA_3, OUTPUT);
  pinMode(CP_CLOCK, OUTPUT);
  pinMode(CP_ACKNOWLEDGE, INPUT);
  attachInterrupt(digitalPinToInterrupt(CP_ACKNOWLEDGE), acknowledged, CHANGE);

  testSetTilePosition();
}

void loop() {
  // put your main code here, to run repeatedly:
}