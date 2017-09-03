#include "button.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"
#include "Loger.h"

extern Mqtt MqttClient;

void Button::SetDefault() {
		startPressing = 0;
		isLongMode = false;
		status = ButtonStatus::BTN_OFF;
}

void Button::InitUnit() {
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
			Loger::Debug("Start pressing");
		}
		else {
			if (startPressing + BUTTON_EXTRA_LONG_PRESS <= now) { // Yes! Button is extra long pressed
				btnValue = BTN_EXTRA_LONG;
				if (!isExtraLongMode) {
					Loger::Debug("Extra Long Detected");
					ProcessUnit(btnValue);
				}
				isExtraLongMode = true;
				isLongMode = false;
				//startPressing = now;
			}
			else {
				if (startPressing + BUTTON_LONG_PRESS <= now) { // Yes! Button is long pressed
					btnValue = BTN_LONG;
					if (!isLongMode) {
						Loger::Debug("Long Detected");
					}
					isShortMode = false;
					isLongMode = true;
					//startPressing = now;
					//ProcessUnit(btnValue);
				}
				else {
					if (startPressing + BUTTON_SHORT_PRESS <= now) { // Yes! Button is already short pressed
						btnValue = BTN_ON;// BTN_SHORT_LONG;
						if (!isShortMode) {
							Loger::Debug("Short during Long");
						}
						isShortMode = true;
					}
					//else {
					//	status = BTN_OFF; //Button pressed less than long press and not released yet
					//	//Debug("Not Long time");
					//}
				}
			}
		}
	}
	else { //Button is released
		if (startPressing != 0) { // Yes! Button had been pressed before
			Loger::Debug("Button pressed(ms):" +String(now - startPressing));
			if (startPressing + BUTTON_WRONG_PRESS > now) { // Button is pressed too short
				btnValue = BTN_OFF;
				Loger::Debug("Too short Detected");
				//	Debug(now - unit->startPressing);
			}
			else {
				if (!isExtraLongMode) {
					if (isLongMode) {
						ProcessUnit(BTN_LONG);
					}
					else {
						ProcessUnit(BTN_ON);
					}
				}
				startPressing = 0;
				isLongMode = false;
				isShortMode = false;
				isExtraLongMode = false;
				ProcessUnit(BTN_OFF);
			}
			startPressing = 0;
		}
		else { //Nothing to happens
			btnValue = BTN_OFF;
			//Debug("Button OFF");
		}
	}

//	if (btnValue != BTN_OFF ) {
//		ProcessUnit(btnValue);
//	}
}

void Button::ProcessUnit(int newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, status, status);
	// Reset button status
	//status = BTN_OFF;
	//MqttClient.PublishUnit(this);

}

void Button::UnitLoop() {
	HandleButton();
};
