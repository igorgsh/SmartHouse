/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/


#include <PZEM004Tv30.h>
#include <MemoryFree.h>
#include <SerialComs.h>
#include <SafeStringStream.h>
#include <SafeStringReader.h>
#include <SafeStringNameSpaceStart.h>
#include <SafeStringNameSpaceEnd.h>
#include <SafeStringNameSpace.h>
#include <SafeString.h>
#include <millisDelay.h>
#include <loopTimer.h>
#include <BufferedOutput.h>
#include <BufferedInput.h>

#include "Contactor.h"
#include <PZEM004T.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "SigmaEEPROM.h"
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "utils.h"
//#include <Ethernet.h>
#include "avr\wdt.h"
//#include "Mqtt.h"
#include "Configuration.h"
#include "Action.h"
#include "Relay.h"
#include <Arduino.h>
#include "definitions.h"
#include "global.h"
#include "Button.h"
#include "Loger.h"
#include "definitions.h"
//#include "ext_global.h"


#include "utils.h"

// the setup function runs once when you press reset or power the board
void setup() {
	wdt_disable();
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
	Serial.println("Serial!");
	//MEMFREE("Start Init");
	SafeString::setOutput(Serial);
	//init random generator
	randomSeed(analogRead(0));

	//initialization of config
	Config.Init();
	Config.Log->Info(F("Board is ready"));
	Config.Log->append(F("Board Id#:")).append(Config.BoardId).Info();
	Config.Log->append(F("IP Address is:")).append(Config.ip[0]).append(".").append(Config.ip[1]).append(".").append(Config.ip[2]).append(".").append(Config.ip[3]).append(".").Info();
	//Set a timer 
	//MsTimer2::set(100, Timer2);
	//MsTimer2::start();
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Config.Log->Info("Enjoy!");
	//MEMFREE("End init");
}

// the loop function runs over and over again until power down or reset
void loop() {
	//while (1);
	static unsigned long tp60 = 0;
	static unsigned long tp30 = 0;
	static unsigned long tp10 = 0;
	static unsigned long tp1 = 0;
	unsigned long now = millis();

	//Config.Log->append("Loop:now=").append(now).Info();
	if ((now-tp60) > (unsigned long)60*1000) { //1 min 
		//MEMFREE("StartLoop1m");
		Config.Loop(60000);
		Config.counter60++;
		tp60 = now;
	}

	if ((now - tp30) > (unsigned long)30 * 1000) { //30 sec 
		Config.Loop(30000);
		Config.counter30++;
		tp30 = now;
	}
	if ((now - tp10) > (unsigned long)10 * 1000) { //10 sec 
		Config.Loop(10000);
		Config.counter10++;
		tp10 = now;
	}
	if ((now - tp1) > (unsigned long)1 * 1000) { //1 sec 
		Config.Loop(1000);
		Config.counter1++;
		tp1 = now;
	}


	Config.Loop(0);

}
