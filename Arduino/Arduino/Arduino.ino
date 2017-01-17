/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/

#include "configuration.h"
#include "action.h"
#include <ArduinoJson.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include "relay.h"
#include "Arduino.h"
#include "definitions.h"
#include "types.h"
#include "global.h"

#include "button.h"
#include "light.h"


#include "rest_module.h"
//#include "process.h"
#include "ext_global.h"
#include "definitions.h"


#include "initdata.h"
void ButtonScan() {
	for (int i = 0; i < NUMBER_OF_BUTTONS && &Buttons[i] != (ButtonUnit*)NULL && Buttons[i].Id[0] != 0; i++) {
		ProcessButton(&(Buttons[i]));
	}

}

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial) {
		delay(10); // wait for serial port to connect. Needed for native USB port only
	}
	//GetInitialConfiguration();
	InitializeServer();
	InitializeData();
	InitPins();
}

// the loop function runs over and over again until power down or reset
void loop() {
	unsigned long startTime = millis();
	// Step 1. Read all buttons
	ButtonScan();
	
	//	Step 2. Listening a server requests
	ProcessServerRequests();

	Log_(D_INFO, "Working time:");
	Log2(D_INFO, (millis() - startTime), DEC);
	Log(D_INFO, "==================================================================");

}
