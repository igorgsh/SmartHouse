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

void Timer2() { //it is started every 100ms
	Config.loop001(); //100ms
	Config.counter001++;
	if (Config.counter001 % 5 == 0) { //500ms
		Config.loop005();
		Config.counter005++;
		if (Config.counter001 % 10 == 0) { //1sec
			Config.loop1();
			Config.counter1++;
			if (Config.counter001 % 600 == 0) { //1min
				Config.loop60();
				Config.counter60++;
				if (Config.counter001 % 3000 == 0) { //5min
					Config.loop300();
					Config.counter300++;
				}
			}
		}
	}
}

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
	MsTimer2::set(100, Timer2);
	MsTimer2::start();

	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Loger::Debug("Enjoy!");
	//Debug2("Ethernet Status is:", Ethernet._state);
}

// the loop function runs over and over again until power down or reset
void loop() {
	
	Config.MainLoop();

}
