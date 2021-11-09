/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/

/*
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
*/

#include "Contactor.h"
#include <PZEM004T.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
//#include <Keyboard.h>
#include "SigmaEEPROM.h"
//#include <MsTimer2.h>
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

#include "ext_global.h"


#include "utils.h"
/*
void Timer2() { //it is started every 100ms
	Config.loop01(); //100ms
	Config.counter01++;
	if (Config.counter01 % 5 == 1) { //500ms
		Config.loop05();
		Config.counter05++;
		if (Config.counter01 % 10 == 2) { //1sec
			Config.loop1();
			Config.counter1++;
			if (Config.counter01 % 600 == 3) { //1min
				Config.loop60();
				Config.counter60++;
				if (Config.counter01 % 3000 == 4) { //5min
					Config.loop300();
					Config.counter300++;
				}
			}
		}
	}
}
*/
// the setup function runs once when you press reset or power the board
void setup() {

	wdt_disable();
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
	
	//init random generator
	randomSeed(analogRead(0));
	
	//initialization of config
	Config.Init();
	Loger::Info("Board is ready");
	Loger::Info("Board Id#:" + String(Config.BoardId));
	Loger::Info("IP Address is:" + PrintIP(Ethernet.localIP()));
	//Set a timer 
	//MsTimer2::set(100, Timer2);
	//MsTimer2::start();
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Loger::Debug("Enjoy!");
}

// the loop function runs over and over again until power down or reset
void loop() {
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
		Config.loop60();
		Config.counter60++;
		tp60 = now;
	}

	Config.MainLoop();

}
