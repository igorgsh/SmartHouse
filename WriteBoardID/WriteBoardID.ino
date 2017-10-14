#include <EEPROM.h>



void setup() {
  // EEPROM.write(Address, ID)
  EEPROM.write(0,0x03);
  int res = EEPROM.read(0);
  Serial.begin(115200);
  Serial.println("Board Id=" + String(res));
}

void loop() {
  // put your main code here, to run repeatedly:

}
