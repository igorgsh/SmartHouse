/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "utils.h"
#include <Ethernet.h>
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


#include "initdata.h"
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
	SerialLog(D_INFO, "Start");
	
	//init random generator
	randomSeed(analogRead(0));
	Config.Init();
	SerialLog_(D_INFO, "Board Id: ");
	SerialLog2(D_INFO, Config.BoardId, HEX);
	if (Config.IsEthernetConnection) {
		SerialLog(D_INFO, "Init Ethernet");
		InitializeServer();
		SerialLog(D_INFO, "Initialize MQTT");
		MqttClient.InitMqtt();
	}
	SerialLog(D_INFO, "Build Configuration");
	Config.BuildConfig();
	MqttClient.SubscribeUnits();
	Loger::Info("Board is ready");
	Loger::Info("Board Id:" + String(Config.BoardId));
	Loger::Info("IP Address is:" + printIP(Ethernet.localIP()));
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	
	//unsigned long startTime = millis();
	//Debug2("Point10:", memoryFree());

	//	Step 1. Listening a server requests
	MqttClient.MqttLoop();
	//Debug2("Point11:", memoryFree());

	// Step 2. Read all buttons
	Config.UnitsLoop();

/*
	Log_(D_INFO, "Working time:");
	Log2(D_INFO, (millis() - startTime), DEC);
	Log(D_INFO, "==================================================================");
*/
}
