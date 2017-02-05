#include "button.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"

extern Mqtt MqttClient;

void Button::SetDefault() {
		startPressing = 0;
		isLongMode = false;
		status = ButtonStatus::BTN_OFF;
}

void Button::InitUnit() {
	Debug("Button Init");
	pinMode(Pin, INPUT);
	digitalWrite(Pin, !lhOn);
	//status = 99;
	MqttClient.PublishUnit(this);
}

/*

ButtonUnit * FindButton(const char* id) {

	ButtonUnit *unit = NULL;

	for (int i = 0; i < NUMBER_OF_BUTTONS && &(Buttons[i]) != NULL; i++) {
		if (strcmp(Buttons[i].Id, id) == 0) {
			unit = &(Buttons[i]);
			break;
		}
		//if (Buttons[i].Id[0] == 0)
		//	break;
	}
	return unit;
}
*/

void Button::HandleButton() {

	byte btnValue;

	unsigned long now = millis();
	btnValue = digitalRead(Pin);
	
	//btnValue = !btnValue; //The buttons are pulled up to HIGH. And switched to GND
	if (btnValue == lhOn) {// button is pressed
		if (startPressing == 0) { // start pressing
			startPressing = now;
			btnValue = BTN_OFF;
		Debug("Start pressing");
		}
		else {
			if (startPressing + BUTTON_LONG_PRESS <= now) { // Yes! Button is long pressed
				btnValue = BTN_LONG;
				Debug("Long Detected");
				isLongMode = true;
				startPressing = now;
			}
			else {
				if (startPressing + BUTTON_SHORT_PRESS <= now) { // Yes! Button is already short pressed
					btnValue = BTN_SHORT_LONG;
					Debug("Short during Long")
				}
				else {
					status = BTN_OFF; //Button pressed less than long press and not released yet
					//Debug("Not Long time");
				}
			}
		}
	}
	else { //Button is released
		if (startPressing != 0) { // Yes! Button had been pressed before
			if (startPressing + BUTTON_WRONG_PRESS > now) { // Button is pressed too short
				btnValue = BTN_OFF;
				Debug("Short Detected");
			//	Debug(now - unit->startPressing);
			}
			else {
				if (isLongMode) { //end of Longmode
					btnValue = BTN_OFF;
					Debug("Long mode was over")
				}
				else {
					btnValue = BTN_ON;
					Debug("Single click");
			//		Debug(now - unit->startPressing);
				}
			}
			startPressing = 0;
			isLongMode = false;
		}
		else { //Nothing to happens
			btnValue = BTN_OFF;
		}
	}

	if (btnValue != BTN_OFF) {
		ProcessUnit(btnValue);
	}
}
void Button::ProcessUnit(byte newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, status, status);
	// Reset button status
	status = BTN_OFF;
	MqttClient.PublishUnit(this);

}

void Button::UnitLoop() {
	HandleButton();
};
