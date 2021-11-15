#include <Arduino.h>

#include <EEPROM.h>




void setup() {
  // EEPROM.write(Address, ID)
  EEPROM.write(0,0x02);
  byte res = EEPROM.read(0);
//  EEPROM.write(0,0x02);
  Serial.begin(115200);
  Serial.println("Board Id=" + String(res));
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1500);                       // wait for a second
  
}
