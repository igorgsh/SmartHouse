#include "button.h"
#include "ext_global.h"
#include "mqtt.h"
#include "Loger.h"
#include "SigmaEEPROM.h"

extern Mqtt MqttClient;

void Button::SetDefault() {
	startPressing = 0;
	isLongMode = false;
	status = ActionType::ACT_OFF;
}

void Button::InitUnit() {
//	Loger::Debug("Init Button:id=" + String(Id) + "; Pin=" + String(Pin));
	pinMode(Pin, INPUT);
	digitalWrite(Pin, !lhOn);
	MqttClient.PublishUnit(this);
}


void Button::HandleButton() {

	byte btnValue;

	unsigned long now = millis();
	btnValue = digitalRead(Pin);

	if (btnValue == lhOn) {// button is pressed
		if (startPressing == 0) { // start pressing
			startPressing = now;
			btnValue = ACT_OFF;
			Loger::Debug("Start pressing[" + String(Id) + "]");
		}
		else {
			if (startPressing + BUTTON_EXTRA_LONG_PRESS <= now) { // Yes! Button is extra long pressed
				btnValue = ACT_EXTRA_LONG;
				if (!isExtraLongMode) {
					Loger::Debug("Extra Long Detected[" + String(Id) + "]");
					HandleFinish(btnValue);
				}
				isExtraLongMode = true;
				isLongMode = false;
				//startPressing = now;
			}
			else {
				if (startPressing + BUTTON_LONG_PRESS <= now) { // Yes! Button is long pressed
					btnValue = ACT_LONG;
					if (!isLongMode) {
						Loger::Debug("Long Detected[" + String(Id) + "]");
					}
					isShortMode = false;
					isLongMode = true;
				}
				else {
					if (startPressing + BUTTON_SHORT_PRESS <= now) { // Yes! Button is already short pressed
						btnValue = ACT_ON;// BTN_SHORT_LONG;
						if (!isShortMode) {
							Loger::Debug("Short during Long[" + String(Id) + "]");
						}
						isShortMode = true;
					}
				}
			}
		}
	}
	else { //Button is released
		if (startPressing != 0) { // Yes! Button had been pressed before
			Loger::Debug("Button [" + String(Id) + "] pressed(ms):" + String(now - startPressing));
			if (startPressing + BUTTON_WRONG_PRESS > now) { // Button is pressed too short
				btnValue = ACT_OFF;
				Loger::Debug("Too short Detected");
			}
			else {
				if (!isExtraLongMode) {
					if (isLongMode) {
						HandleFinish(ACT_LONG);
					}
					else {
						HandleFinish(ACT_ON);
					}
				}
				startPressing = 0;
				isLongMode = false;
				isShortMode = false;
				isExtraLongMode = false;
				HandleFinish(ACT_OFF);
			}
			startPressing = 0;
		}
		else { //Nothing to happens
			btnValue = ACT_OFF;
			//Debug("Button OFF");
		}
	}

	//	if (btnValue != ACT_OFF ) {
	//		ProcessUnit(btnValue);
	//	}
}

void Button::HandleFinish(int newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, newStatus);

}

void Button::ProcessUnit(ActionType event) {
	Config.ProcessAction(Id, event);
	status = ACT_OFF;
	MqttClient.PublishUnit(this);

}

void Button::UnitLoop() {
	HandleButton();
};

bool Button::Compare(Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::BUTTON) return false;
	Button *tu = (Button*)u;
	Loger::Debug("Compare Buttons:" + String(Id == tu->Id) + ":" +String(Type == tu->Type) +":" + String(Pin == tu->Pin) + ":" + String(lhOn == tu->lhOn) + ":" + String(status == tu->status) + "#");
	return (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn &&
		status == tu->status
		);
}


void Button::ReadFromEEPROM(uint16_t addr) {
//	bool res = true;

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);
	lhOn = SigmaEEPROM::Read8(addr + 3);
	status = SigmaEEPROM::Read8(addr + 4);

}

void Button::WriteToEEPROM(uint16_t addr) {
	//bool res = true;

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);
	SigmaEEPROM::Write8(addr + 3, lhOn);
	SigmaEEPROM::Write8(addr + 4, status);

}

void Button::ConfigField(JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("lhOn")) {
		lhOn = jsonList["lhOn"];
	}
	if (jsonList.containsKey("status")) {
		status = jsonList["status"];
	}
}


void const Button::print(const char* header, DebugLevel level) {
	String str0 = "";

	if (header != NULL) {
		str0 = header;
	}
	str0 += "Id:";
	str0 += String((unsigned int)Id, DEC);
	str0 += ";Type:";
	str0 += String((char)Type);
	str0 += ";Pin:";
	str0 += String((unsigned int)Pin, DEC);
	str0 += ";lhOn:";
	str0 += String((unsigned int)lhOn, DEC);
	str0 += ";subscription:";
	str0 += (isSubscribed ? "true" : "false");
	str0 += " @";
	Loger::Log(level, str0);
}
