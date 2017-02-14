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
void Timer2() {
	static byte counter = 0;
	unsigned long now = millis();

	static bool isDataReady = false;
//	Serial.print("Counter=");
//	Serial.println(counter);

	if (counter == 0) {
		Configuration.requestTemperatures();
		isDataReady = false;
		for (int i = 0; i < Configuration.getNumberTemp(); i++) {
			Configuration.tempSensors[i].setPrepareData(true);
		}
		counter++;
	}
	else if (!isDataReady) {
		if (counter == 9) {
			for (int i = 0; i < Configuration.getNumberTemp(); i++) {
				if (Configuration.tempSensors[i].getPrepareData()) {
					Configuration.tempSensors[i].setError(ErrorCode::SENSOR_DISCONNECTED);
				}
			}
			// some sensor didn't provides a data
			counter = 0;
		}
		else {
			int cnt = 0;
			for (int i = 0; i < Configuration.getNumberTemp(); i++) {
//				float temp=-999.99;
				if (Configuration.tempSensors[i].checkDataReady()) {
					Configuration.tempSensors[i].setPrepareData(false);
					//Serial.println(i);
					cnt++;
				}
			}
			isDataReady = (cnt == Configuration.getNumberTemp());
		}
		counter++;
	}
	else {
		counter++;
		if (counter == 10)
			counter = 0;
	}
}

void setup() {
	Serial.begin(115200);
	Configuration.begin();

	// locate devices on the bus
	Serial.print("Locating devices...");
	Serial.print("Found ");
	Serial.print(Configuration.getNumberTemp(), DEC);
	Serial.println(" devices.");
	MsTimer2::set(100, Timer2);
	MsTimer2::start();
	delay(2000);
}

// the loop function runs over and over again until power down or reset
void loop() {

	for (int i = 0; i < Configuration.getNumberTemp(); i++) {
		Serial.print(Configuration.tempSensors[i].getLabel());
		Serial.print(":");
		switch (Configuration.tempSensors[i].getError()) {
		case ErrorCode::NO_ERROR: {
			Serial.println(Configuration.tempSensors[i].getValue());
			break;
		}
		case ErrorCode::HIGH_VALUE: {
			Serial.println("HIGH");
			break;
		}
		case ErrorCode::LOW_VALUE: {
			Serial.println("LOW");
			break;
		}
		case ErrorCode::SENSOR_DISCONNECTED: {
			Serial.println("CONN");
			break;
		}
		}
	}
	delay(2000);
}
