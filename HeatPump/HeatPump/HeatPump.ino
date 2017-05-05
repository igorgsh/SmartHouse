/*
 Name:		HeatPump.ino
 Created:	13.02.17 18:58:02
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <SD.h>
#include <SPI.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include <MsTimer2.h>
//#include <DallasTemperature.h>
#include <OneWire.h>
#include "DT.h"
#include "Configuration.h"
#include "Definitions.h"


DebugLevel dLevel=D_ALL;


#define SDCARD_SS	4
#define LED_PIN	13

//#define ONEWIRE_PIN	40

//OneWire wire(ONEWIRE_PIN);
//DT tempSensors(&wire);
Configuration Config;
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
	static byte counter100 = 0;

	Config.loop(counter100);
	counter100++;
}

void setup() {
	//Configure Serial port and SD card
	Serial.begin(115200);
	SD.begin(SDCARD_SS);

	// Initialize configuration
	Config.begin();

	// Prepare the light indicator 
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	// locate devices on the bus
	Serial.print("Locating devices...");
	Serial.print("Found ");
	Serial.print(Config.getNumberTemp() + Config.getNumberCont(), DEC);
	Serial.println(" devices.");

	//Set a timer 
	MsTimer2::set(100, Timer2);
	MsTimer2::start();

//	delay(500);
}
void Alarm(bool onOff) {
	// some buzzer/flash/ should be here
	digitalWrite(LED_PIN, onOff);
}
// the loop function runs over and over again until power down or reset
void loop() {
/*
	bool isError = false;

	for (int i = 0; i < Config.getNumberTemp(); i++) {
		Serial.print(Config.tempSensors[i].getLabel());
		Serial.print(":");
		Serial.print(Config.tempSensors[i].getValue());
		Serial.print(":");
		Serial.print(Config.tempSensors[i].isWaitingStart() ? "W:" : "N:");
		Serial.print(Config.tempSensors[i].isCritical() ? "*:" : " :");
		switch (Config.tempSensors[i].getError()) {
		case ErrorCode::NO_ERROR: {
			//Serial.println(Configuration.tempSensors[i].getValue());
			Serial.println();
			break;
		}
		case ErrorCode::HIGH_VALUE: {
			Serial.println("HIGH");
			isError = Config.tempSensors[i].isCritical();
			break;
		}
		case ErrorCode::LOW_VALUE: {
			Serial.println("LOW");
			isError = Config.tempSensors[i].isCritical();
			break;
		}
		case ErrorCode::SENSOR_DISCONNECTED: {
			Serial.println("CONN");
			isError = Config.tempSensors[i].isCritical();
			break;
		}
		}
	}
	for (int i = 0; i < Config.getNumberCont(); i++) {
		Serial.print(Config.contacts[i].getLabel());
		Serial.print(":");
		Serial.print(Config.contacts[i].getIntValue());
		Serial.print(":");
		Serial.print(Config.contacts[i].isWaitingStart() ? "W:" : "N:");
		Serial.print(Config.contacts[i].isCritical() ? "*:" : ":");
		switch (Config.contacts[i].getError()) {
		case ErrorCode::NO_ERROR: {
//			Serial.println(Configuration.contacts[i].getIntValue());
			Serial.println();
			break;
		}
		case ErrorCode::HIGH_VALUE: 
		case ErrorCode::LOW_VALUE: 
		case ErrorCode::SENSOR_DISCONNECTED: {
			Serial.println("CONN");
			isError = Config.contacts[i].isCritical();
			break;
		}
		}
	}
	Alarm(isError);
	*/
//	delay(1000);
}
