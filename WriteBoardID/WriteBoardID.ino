#include <Arduino.h>

#include <EEPROM.h>




void setup() {
  // EEPROM.write(Address, ID)
	byte id = 0x02;
	byte ip[4] = { 192,168,0,90 };
	byte mqttIp = 34;

	ip[3] += id;
	EEPROM.write(0,id);

	EEPROM.write(1, ip[0]);
	EEPROM.write(2, ip[1]);
	EEPROM.write(3, ip[2]);
	EEPROM.write(4, ip[3]);

	EEPROM.write(5, mqttIp);

	byte res = EEPROM.read(0);
	Serial.begin(115200);
	Serial.print("Board Id=");
	Serial.print(res);
	res = EEPROM.read(1);
	Serial.print("; ip=");
	Serial.print(res);
	res = EEPROM.read(2);
	Serial.print(".");
	Serial.print(res);
	res = EEPROM.read(3);
	Serial.print(".");
	Serial.print(res);
	res = EEPROM.read(4);
	Serial.print(".");
	Serial.print(res);
	res = EEPROM.read(5);
	Serial.print("; mqtt=");
	Serial.print(res);
	Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1500);                       // wait for a second
  
}
