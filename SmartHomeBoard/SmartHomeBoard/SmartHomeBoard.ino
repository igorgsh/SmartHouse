/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/


#include "definitions.h"
#include "ext_global.h"
#include "global.h"

#include "Contactor.h"
#include <PZEM004T.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "SigmaEEPROM.h"
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "utils.h"
#include "avr\wdt.h"
#include "Mqtt.h"
#include "configuration.h"
#include "Action.h"
#include "Relay.h"
#include <Arduino.h>
#include "Button.h"
#include "Loger.h"



#include "utils.h"

// the setup function runs once when you press reset or power the board
void setup() {

	wdt_disable();
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
	Loger::LogMessage.reserve(200);
	//init random generator
	randomSeed(analogRead(0));
	
	//initialization of config
	Config.Init();
	Loger::LogMessage = F("Board is ready");
	Loger::Info();
	Loger::LogMessage = F("Board Id#:");
	Loger::LogMessage += Config.BoardId;
	Loger::Info();
	Loger::LogMessage = F("IP Address is:");
	PrintIP(Ethernet.localIP(), Loger::LogMessage);
	Loger::Info();
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Loger::LogMessage = F("Enjoy!");
	Loger::Debug();
}

// the loop function runs over and over again until power down or reset
void loop() {
	static unsigned long tp60 = 0;
	static unsigned long tp30 = 0;
	unsigned long now = millis();

	if ((now - tp30) > (unsigned long)30 *  1000) { //30 sec 
		Config.loop30();
		Config.counter30++;
		tp30 = now;
	}

	if ((now-tp60) > (unsigned long)60*1000) { //1 min 
		Config.loop60();
		Config.counter60++;
		tp60 = now;
	}

	Config.MainLoop();

}
