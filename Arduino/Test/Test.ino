/*
 Name:		Test.ino
 Created:	02.02.17 18:27:22
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <EEPROM.h>
void setup() {
	Serial.begin(115200);
	//EEPROM.write(0, 1);
	for (int i = 0; i < 10; i++) {
		Serial.println(EEPROM.read(i), HEX);
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
