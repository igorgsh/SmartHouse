/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/


<<<<<<< HEAD
#include "definitions.h"
#include "ext_global.h"
#include "global.h"
=======
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
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516

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
//#include "Mqtt.h"
#include "Configuration.h"
#include "Action.h"
#include "Relay.h"
#include <Arduino.h>
#include "Button.h"
#include "Loger.h"
<<<<<<< HEAD
#include "utils.h"

String Loger::LogMessage;

=======
#include "definitions.h"
//#include "ext_global.h"


#include "utils.h"

>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
// the setup function runs once when you press reset or power the board
void setup() {
	wdt_disable();
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
<<<<<<< HEAD
	Loger::LogMessage.reserve(200);
=======
	Serial.println("Serial!");
	//MEMFREE("Start Init");
	SafeString::setOutput(Serial);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	//init random generator
	randomSeed(analogRead(0));

	//initialization of config
	Config.Init();
<<<<<<< HEAD
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
=======
	Config.Log->Info(F1("Board is ready"));
	Config.Log->append(F1("Board Id#:")).append(Config.BoardId).Info();
	Config.Log->append(F1("IP Address is:")).append(Config.strIP).Info();
	//Set a timer 
	//MsTimer2::set(100, Timer2);
	//MsTimer2::start();
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Config.Log->Info("Enjoy!");
	//MEMFREE("End init");
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}

// the loop function runs over and over again until power down or reset
void loop() {
	//while (1);
	static unsigned long tp60 = 0;
	static unsigned long tp30 = 0;
	static unsigned long tp10 = 0;
	static unsigned long tp1 = 0;
	unsigned long now = millis();


	if ((now-tp60) > (unsigned long)60*1000) { //1 min 
		//MEMFREE("StartLoop1m");
		Config.loop60();
		Config.counter60++;
		tp60 = now;
	}

	if ((now - tp30) > (unsigned long)30 * 1000) { //30 sec 
		Config.loop30();
		Config.counter30++;
		tp30 = now;
	}
	if ((now - tp10) > (unsigned long)10 * 1000) { //10 sec 
		Config.loop10();
		Config.counter10++;
		tp10 = now;
	}
	if ((now - tp1) > (unsigned long)1 * 1000) { //1 sec 
		Config.loop1();
		Config.counter1++;
		tp1 = now;
	}

	Config.MainLoop();

}
