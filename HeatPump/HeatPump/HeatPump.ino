/*
 Name:		HeatPump.ino
 Created:	13.02.17 18:58:02
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <MsTimer2.h>
//#include <DallasTemperature.h>
#include <OneWire.h>
#include "DT.h"
#include "Config.h"

#define LED_PIN	13
#define ONEWIRE_PIN	40

//OneWire wire(ONEWIRE_PIN);
//DT tempSensors(&wire);
Config Configuration;
/*
void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}
int numbSensors = 0;
*/
void Timer2() { //it is started every 100ms
	static byte counter = 0;

	Configuration.loop(counter);
	
	counter++;
}

void setup() {
	Serial.begin(115200);
	Configuration.begin();
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	// locate devices on the bus
	Serial.print("Locating devices...");
	Serial.print("Found ");
	Serial.print(Configuration.getNumberTemp() + Configuration.getNumberCont(), DEC);
	Serial.println(" devices.");
	MsTimer2::set(100, Timer2);
	MsTimer2::start();
	delay(500);
}
void Alarm(bool onOff) {
	// some buzzer/flash/ should be here
	digitalWrite(LED_PIN, onOff);
}
// the loop function runs over and over again until power down or reset
void loop() {

	bool isError = false;

	for (int i = 0; i < Configuration.getNumberTemp(); i++) {
		Serial.print(Configuration.tempSensors[i].getLabel());
		Serial.print(":");
		Serial.print(Configuration.tempSensors[i].getValue());
		Serial.print(":");
		Serial.print(Configuration.tempSensors[i].isWaitingStart() ? "W:" : "N:");
		Serial.print(Configuration.tempSensors[i].isCritical() ? "*:" : " :");
		switch (Configuration.tempSensors[i].getError()) {
		case ErrorCode::NO_ERROR: {
			//Serial.println(Configuration.tempSensors[i].getValue());
			Serial.println();
			break;
		}
		case ErrorCode::HIGH_VALUE: {
			Serial.println("HIGH");
			isError = Configuration.tempSensors[i].isCritical();
			break;
		}
		case ErrorCode::LOW_VALUE: {
			Serial.println("LOW");
			isError = Configuration.tempSensors[i].isCritical();
			break;
		}
		case ErrorCode::SENSOR_DISCONNECTED: {
			Serial.println("CONN");
			isError = Configuration.tempSensors[i].isCritical();
			break;
		}
		}
	}
	for (int i = 0; i < Configuration.getNumberCont(); i++) {
		Serial.print(Configuration.contacts[i].getLabel());
		Serial.print(":");
		Serial.print(Configuration.contacts[i].getIntValue());
		Serial.print(":");
		Serial.print(Configuration.contacts[i].isWaitingStart() ? "W:" : "N:");
		Serial.print(Configuration.contacts[i].isCritical() ? "*:" : ":");
		switch (Configuration.contacts[i].getError()) {
		case ErrorCode::NO_ERROR: {
//			Serial.println(Configuration.contacts[i].getIntValue());
			Serial.println();
			break;
		}
		case ErrorCode::HIGH_VALUE: 
		case ErrorCode::LOW_VALUE: 
		case ErrorCode::SENSOR_DISCONNECTED: {
			Serial.println("CONN");
			isError = Configuration.contacts[i].isCritical();
			break;
		}
		}
	}
	Alarm(isError);
	delay(1000);
}
