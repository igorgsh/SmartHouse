/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/
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

String printIP(IPAddress ip) {
	String s = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
	return s;
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
	Loger::Info("IP Address is:" + printIP(Ethernet.localIP()));
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	//Loger::Info("Ethernet Status is:" + String(Ethernet._state));
	//Debug2("Ethernet Status is:", Ethernet._state);
}

// the loop function runs over and over again until power down or reset
void loop() {
	
	//unsigned long startTime = millis();
	//Debug2("Point10:", memoryFree());

	Config.MainLoop();

/*
	Log_(D_INFO, "Working time:");
	Log2(D_INFO, (millis() - startTime), DEC);
	Log(D_INFO, "==================================================================");
*/
}
