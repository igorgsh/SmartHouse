/*
 Name:		Arduino.ino
 Created:	11.01.17 22:04:42
 Author:	Igor Shevchenko
*/

#include "Arduino.h"
#include "definitions.h"
#include "peripheral.h"
#include "global.h"

#include "button.h"
#include "light.h"


#include "rest_module.h"
//#include "process.h"
#include "ext_global.h"


#include "initdata.h"

void InitPins() {

	// Initialize buttons and pull it up
	for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
		if (Buttons[i].Pin != 0) {
			pinMode(Buttons[i].Pin, INPUT);
			digitalWrite(Buttons[i].Pin, HIGH);
		}
	}

	// Initialize lights
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++) {
		if (Lights[i].Pin != 0) {
			pinMode(Lights[i].Pin, OUTPUT);
			LightSet(Lights[i].Id, Lights[i].status);

		}
	}


}


// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	InitializeData();
	InitPins();
	Debug("Size Buttons=" + sizeof(ButtonUnit));
}

// the loop function runs over and over again until power down or reset
void loop() {
	unsigned long startTime = millis();
	char buf[50];
	sprintf(buf,"Size Buttons=%u" ,sizeof(ButtonUnit));
	Debug(buf);
	// Step 1. Read all buttons
	for (int i = 0; i < NUMBER_OF_BUTTONS && &Buttons[i] != (ButtonUnit*)NULL && Buttons[i].Id[0] != 0; i++) {
//		sprintf(buf, "Button:%u, id=%s, starting=%lu", i, Buttons[i].Id, Buttons[i].startPressing);
//		Log(D_DEBUG, buf);
		ProcessButton(&(Buttons[i]));
	}

//	Log(D_DEBUG, "Buttons have been processed");
	ProcessServerRequests();
	sprintf(buf, "Working time: %lu ms", millis() - startTime);
	Log(D_INFO, buf);
	Log(D_INFO, "==================================================================");

}
