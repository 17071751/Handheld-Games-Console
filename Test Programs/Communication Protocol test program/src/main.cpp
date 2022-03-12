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

uint16_t test_pixels[64] = {
  51371, 15390, 4740, 55312, 45067, 50205, 4292, 48102,
  27368, 13472, 37702, 50287, 39839, 20183, 14711, 60542,
  54907, 49235, 36958, 51117, 43692, 50835, 29312, 11016,
  630, 26059, 60461, 29565, 32333, 16163, 47608, 4827,
  64799, 57395, 5368, 48941, 18770, 37072, 45476, 38501,
  31456, 14253, 56937, 28155, 61514, 13793, 60557, 63599,
  19395, 3656, 63120, 38318, 61970, 11513, 59953, 51821,
  9296, 24529, 19988, 59102, 5213, 59918, 12669, 10179
};

bool acknowledge = false;

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

void testSetTilePosition() {
  // select "Set Tile position"
  Serial.println("select \"Set Tile position\"");
  writeMessage(1);
  // Write to tile map
  Serial.println("Write to tile map");
  writeMessage(0);
  // Write x of 26
  Serial.println("Write x of 26 part 1");
  writeMessage(26 & 0xF);
  Serial.println("Write x of 26 part 2");
  writeMessage(26 >> 4);
  // Write y of 17
  Serial.println("Write y of 17 part 1");
  writeMessage(17 & 0xF);
  Serial.println("Write y of 17 part 2");
  writeMessage(17 >> 4);
  // Write tile ID of 36
  Serial.println("Write tile ID of 36 part 1");
  writeMessage(36 & 0xF);
  Serial.println("Write tile ID of 36 part 2");
  writeMessage(36 >> 4);
  Serial.println("Write tile ID of 36 part 3");
  writeMessage(0);
  Serial.println("Write tile ID of 36 part 4");
  writeMessage(0);
}

void testSetTileData() {
  // select "Set Tile data"
  Serial.println("select \"Set Tile data\"");
  writeMessage(2);
  // Write tile ID of 36
  Serial.println("Write tile ID of 36 part 1");
  writeMessage(36 & 0xF);
  Serial.println("Write tile ID of 36 part 2");
  writeMessage(36 >> 4);
  Serial.println("Write tile ID of 36 part 3");
  writeMessage(0);
  Serial.println("Write tile ID of 36 part 4");
  writeMessage(0);

  // Write pixels
  Serial.println("Writing pixels");
  for (int i = 0; i < 64; ++i) {
    // Write pixel
    Serial.printf("Write pixel %d part 1\n", i);
    writeMessage(test_pixels[i] & 0xF);
    Serial.println("Write pixel part 2");
    writeMessage((test_pixels[i] >> 4) & 0xF);
    Serial.println("Write pixel part 3");
    writeMessage((test_pixels[i] >> 8) & 0xF);
    Serial.println("Write pixel part 4");
    writeMessage((test_pixels[i] >> 12) & 0xF);
  }
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

  //testSetTilePosition();
  testSetTileData();
}

void loop() {
  // put your main code here, to run repeatedly:
}