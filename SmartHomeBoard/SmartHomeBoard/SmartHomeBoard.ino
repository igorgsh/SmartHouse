/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/


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
#include "Mqtt.h"
#include "configuration.h"
#include "Action.h"
#include "Relay.h"
#include <Arduino.h>
#include "definitions.h"
#include "global.h"
#include "Button.h"
#include "Loger.h"
#include "definitions.h"
#include "ext_global.h"


#include "utils.h"

// the setup function runs once when you press reset or power the board
void setup() {
	wdt_disable();
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
	MEMFREE("Start Init");
	SafeString::setOutput(Serial);
	//init random generator
	randomSeed(analogRead(0));

	//initialization of config
	Config.Init();
	Log.Info(F1("Board is ready"));
	Log.append(F1("Board Id#:")).append(Config.BoardId).Info();
	Log.append(F1("IP Address is:")).append(Config.strIP).Info();
	//Set a timer 
	//MsTimer2::set(100, Timer2);
	//MsTimer2::start();
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Log.Info("Enjoy!");
	MEMFREE("End init);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//while (1);
	static unsigned long tp60 = 0;
	static unsigned long tp30 = 0;
	unsigned long now = millis();
	//Loger::Debug("Loop");

	if ((now - tp30) > (unsigned long)30 *  1000) { //30 sec 
		Config.loop30();
		Config.counter30++;
		tp30 = now;
	}

	if ((now-tp60) > (unsigned long)60*1000) { //1 min 
		MEMFREE("StartLoop1m");
		Config.loop60();
		Config.counter60++;
		tp60 = now;
	}

	Config.MainLoop();

}
