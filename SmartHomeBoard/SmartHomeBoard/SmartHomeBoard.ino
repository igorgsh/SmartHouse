/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/
#include <MsTimer2.h>
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


//#include "initdata.h"
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
	Loger::Info("Board Id:" + String(Config.BoardId));
	Loger::Info("IP Address is:" + PrintIP(Ethernet.localIP()));
	//Set a timer 
	//MsTimer2::set(100, Timer2);
	//MsTimer2::start();
	//Loger::Debug("Point 1");
	pinMode(13, OUTPUT);
	//Loger::Debug("Point 2");
	digitalWrite(13, HIGH);
	Loger::Debug("Enjoy!");
	//Debug2("Ethernet Status is:", Ethernet._state);
}

// the loop function runs over and over again until power down or reset
void loop() {
	static unsigned long tp60 = 0;
	unsigned long now = millis();
	//Loger::Debug("Loop");

	if ((now-tp60) > (unsigned long)1*60*1000) { //1 min 
		Config.loop60();
		Config.counter60++;
		tp60 = now;
	}
	Config.MainLoop();

}
