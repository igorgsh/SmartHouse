#include "button.h"
#include "process.h"
#include "ext_global.h"

void ProcessButton(ButtonUnit * unit) {

	byte btnValue;

	unsigned long now = millis();
	btnValue = digitalRead(unit->Pin);

	btnValue = !btnValue; //The buttons are pulled up to HIGH. And switched to GND
	if (btnValue == 1) {// button is pressed
		if (unit->startPressing == 0) { // start pressing
			unit->startPressing = now;
		btnValue = BTN_OFF;
		//Log(D_DEBUG, "Start pressing");
		}
		else {
			if (unit->startPressing + BUTTON_LONG_PRESS <= now) { // Yes! Button is long pressed
				btnValue = BTN_LONG;
				Debug("Long Detected");
				unit->isLongMode = true;
				unit->startPressing = now;
			}
			else {
				if (unit->startPressing + BUTTON_SHORT_PRESS <= now) { // Yes! Button is already short pressed
					btnValue = BTN_SHORT_LONG;
					Debug("Short during Long")
				}
				else {
					btnValue = BTN_OFF; //Button pressed less than long press and not released yet
					Debug("Not Long time");
				}
			}
		}
	}
	else { //Button is released
		if (unit->startPressing != 0) { // Yes! Button had been pressed before
			if (unit->startPressing + BUTTON_WRONG_PRESS > now) { // Button is pressed too short
				btnValue = BTN_OFF;
				Debug("Short Detected");
			//	Debug(now - unit->startPressing);
			}
			else {
				if (unit->isLongMode) { //end of Longmode
					btnValue = BTN_OFF;
					Debug("Long mode was over")
				}
				else {
					btnValue = BTN_ON;
					Debug("Single click");
			//		Debug(now - unit->startPressing);
				}
			}
			unit->startPressing = 0;
			unit->isLongMode = false;
		}
		else { //Nothing to happens
			btnValue = BTN_OFF;
		}
	}
	Debug("Button is:" + (32+btnValue));
	ProcessAction(unit->Id, btnValue, 0, 0);

}
