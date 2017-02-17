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

	static bool isTempReady = false;
	static bool isContReady = false;

	if (counter == 0) { //first loop - request for data
		isTempReady = false;
		for (int i = 0; i < Configuration.getNumberTemp(); i++) {
			Configuration.tempSensors[i].requestTemperatures();
			Configuration.tempSensors[i].setPrepareData(true);
		}
		isContReady = false;
		for (int i = 0; i < Configuration.getNumberCont(); i++) {
			Configuration.contacts[i].requestState();
			Configuration.contacts[i].setPrepareData(true);
		}
		counter++;
	}
	else if (!isTempReady) { // waiting for data ready
		if (counter == 9) { // the last loop. All sensors which didn't ready marked as Disconnected
							// some sensor didn't provides a data
			for (int i = 0; i < Configuration.getNumberTemp(); i++) {
				if (Configuration.tempSensors[i].getPrepareData()) {
					Configuration.tempSensors[i].setError(ErrorCode::SENSOR_DISCONNECTED);
					Configuration.tempSensors[i].ErrorCounter++;
				}
			}
		}
		else {
			int cnt = 0;
			for (int i = 0; i < Configuration.getNumberTemp(); i++) {
				if (Configuration.tempSensors[i].checkDataReady()) { //data is ready
					Configuration.tempSensors[i].setPrepareData(false);
					cnt++;
				}
			}
			isTempReady = (cnt == Configuration.getNumberTemp());
		}
	}
	else if (!isContReady) {
		if (counter == 9) { // the last loop. All sensors which didn't ready marked as Disconnected
							// some sensor didn't provides a data
			for (int i = 0; i < Configuration.getNumberCont(); i++) {
				if (Configuration.contacts[i].getPrepareData()) {
					Configuration.contacts[i].setError(ErrorCode::SENSOR_DISCONNECTED);
					Configuration.contacts[i].ErrorCounter++;
				}
			}
		}
		else {
			int cnt = 0;
			for (int i = 0; i < Configuration.getNumberCont(); i++) {
				if (Configuration.contacts[i].checkDataReady()) { //data is ready
					Configuration.contacts[i].setPrepareData(false);
					cnt++;
				}
			}
			isContReady = (cnt == Configuration.getNumberCont());
		}
	}
	counter++;
	if (counter == 10) {
		counter = 0;
	}
}

void setup() {
	Serial.begin(115200);
	Configuration.begin();
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	for (int i = 0; i < Configuration.getNumberCont(); i++) {
		digitalWrite(Configuration.contacts[i].getPin(), Configuration.contacts[i].getNormal());
	}
	// locate devices on the bus
	Serial.print("Locating devices...");
	Serial.print("Found ");
	Serial.print(Configuration.getNumberTemp(), DEC);
	Serial.println(" devices.");
	MsTimer2::set(100, Timer2);
	MsTimer2::start();
	delay(2000);
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
		switch (Configuration.tempSensors[i].getError()) {
		case ErrorCode::NO_ERROR: {
			Serial.println(Configuration.tempSensors[i].getValue());
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
		switch (Configuration.contacts[i].getError()) {
		case ErrorCode::NO_ERROR: {
			Serial.println(Configuration.contacts[i].getIntValue());
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
	delay(2000);
}
