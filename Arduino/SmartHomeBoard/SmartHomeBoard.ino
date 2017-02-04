/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/

#include "avr\wdt.h"
#include "mqtt.h"
#include <PubSubClient.h>
#include "configuration.h"
#include "action.h"
#include <ArduinoJson.h>
#include <Dns.h>
#include <Dhcp.h>
#include "relay.h"
#include "Arduino.h"
#include "definitions.h"
#include "types.h"
#include "global.h"
#include "button.h"
#include "light.h"
#include "Loger.h"

#include "ext_global.h"
#include "definitions.h"


#include "initdata.h"
/*
void ButtonScan() {
	for (int i = 0; i < NUMBER_OF_BUTTONS && &Buttons[i] != (ButtonUnit*)NULL && Buttons[i].Id[0] != 0; i++) {
		ProcessButton(&(Buttons[i]));
	}

}
*/
//void callback(char* topic, byte* payload, unsigned int length);


// the setup function runs once when you press reset or power the board
void setup() {
	wdt_disable();
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
	//GetInitialConfiguration();
	SerialLog(D_INFO, "Start");
	//init random generator
	randomSeed(analogRead(0));
	//EEPROM.write(0, 0);
	Config.Init();
	SerialLog_(D_INFO, "Board Id: ");
	SerialLog2(D_INFO, Config.BoardId, HEX);
	SerialLog(D_INFO, "Init Ethernet")
	InitializeServer();
	SerialLog(D_INFO, "Initialize MQTT");
	MqttClient.InitMqtt();
	SerialLog(D_INFO, "Build Configuration");
	Config.BuildConfig();
	Loger::Info("Board is ready");
	//Log(D_INFO, "Initialize units");
	//InitializeData();
	//Log(D_INFO, "Initialize pins");
	//InitPins();
	//Log(D_INFO, "Initialize MQTT");
	//InitMqtt();
}

// the loop function runs over and over again until power down or reset
void loop() {
	//unsigned long startTime = millis();
	MqttClient.MqttLoop();

		// Step 1. Read all buttons
		//ButtonScan();

		//	Step 2. Listening a server requests
		//ProcessServerRequests();
/*
	Log_(D_INFO, "Working time:");
	Log2(D_INFO, (millis() - startTime), DEC);
	Log(D_INFO, "==================================================================");
*/
}
