#include "button.h"
<<<<<<< HEAD
#include "ext_global.h"
#include "mqtt.h"
//#include "Loger.h"
=======
//#include "mqtt.h"
#include "Loger.h"
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
#include "SigmaEEPROM.h"
#include "Configuration.h"

extern Configuration Config;

void Button::SetDefault() {
	startPressing = 0;
	isLongMode = false;
	status = ActionType::ACT_OFF;
}

void Button::InitUnit() {
	pinMode(Pin, INPUT);
	digitalWrite(Pin, !lhOn);
	Config.MqttClient->PublishUnit(this);
}


void Button::HandleButton() {

	byte btnValue;

	unsigned long now = millis();
	btnValue = digitalRead(Pin);

	if (btnValue == lhOn) {// button is pressed
		if (startPressing == 0) { // start pressing
			startPressing = now;
			btnValue = ACT_OFF;
		}
		else {
			if (startPressing + BUTTON_EXTRA_LONG_PRESS <= now) { // Yes! Button is extra long pressed
				btnValue = ACT_EXTRA_LONG;
				if (!isExtraLongMode) {
					HandleFinish(btnValue);
				}
				isExtraLongMode = true;
				isLongMode = false;
				//startPressing = now;
			}
			else {
				if (startPressing + BUTTON_LONG_PRESS <= now) { // Yes! Button is long pressed
					btnValue = ACT_LONG;
					isShortMode = false;
					isLongMode = true;
				}
				else {
					if (startPressing + BUTTON_SHORT_PRESS <= now) { // Yes! Button is already short pressed
						btnValue = ACT_ON;// BTN_SHORT_LONG;
						isShortMode = true;
					}
				}
			}
		}
	}
	else { //Button is released
		if (startPressing != 0) { // Yes! Button had been pressed before
			if (startPressing + BUTTON_WRONG_PRESS > now) { // Button is pressed too short
				btnValue = ACT_OFF;
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
		}
	}
}

void Button::HandleFinish(int newStatus) {
	status = newStatus;
	Config.MqttClient->PublishUnit(this);
	Config.ProcessAction(Id, newStatus);

}

void Button::ProcessUnit(ActionType event) {
	Config.ProcessAction(Id, event);
	status = ACT_OFF;
	Config.MqttClient->PublishUnit(this);

}

void Button::UnitLoop() {
	HandleButton();
};

bool Button::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::BUTTON) return false;
	Button *tu = (Button*)u;
	
	bool res = 
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn &&
		parentId == tu->parentId
		;
	return res;
}


void Button::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read16(addr);
	Type = SigmaEEPROM::Read8(addr + 2);
	Pin = SigmaEEPROM::Read8(addr + 3);
	lhOn = SigmaEEPROM::Read8(addr + 4);
	parentId = SigmaEEPROM::Read16(addr + 5); //5-6

}

void Button::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write16(addr, Id);
	SigmaEEPROM::Write8(addr + 2, Type);
	SigmaEEPROM::Write8(addr + 3, Pin);
	SigmaEEPROM::Write8(addr + 4, lhOn);
	SigmaEEPROM::Write16(addr + 5 , Id); //5-6
}

void Button::ConfigField(const JsonObject& jsonList) {
<<<<<<< HEAD
	if (jsonList.containsKey(F("Pin"))) {
		Pin = jsonList[F("Pin")];
=======
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	}
	if (jsonList.containsKey(F("lhOn"))) {
		lhOn = jsonList[F("lhOn")];
	}
	if (jsonList.containsKey(F("status"))) {
		status = jsonList["status"];
	}
	if (jsonList.containsKey("parentId")) {
		parentId = jsonList["parentId"];
	}
}


void const Button::print(const char* header, DebugLevel level) {
<<<<<<< HEAD

	if (header != NULL) {
		Loger::LogMessage = header;
	}
	Loger::LogMessage += F("Id:");
	Loger::LogMessage += (unsigned int)Id;
	Loger::LogMessage += F(";Type:");
	Loger::LogMessage += (char)Type;
	Loger::LogMessage += F(";Pin:");
	Loger::LogMessage += (unsigned int)Pin;
	Loger::LogMessage += F(";lhOn:");
	Loger::LogMessage += (unsigned int)lhOn;
	Loger::LogMessage += F(";subscription:");
	Loger::LogMessage += (isSubscribed ? F("true") : F("false"));
	Loger::LogMessage += F("@");
	Loger::Log(level);
=======
	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F1("Id:")).append((unsigned int)Id);
	Config.Log->append(F1(";Type:")).append((char)Type);
	Config.Log->append(F1(";Pin:")).append((unsigned int)Pin);
	Config.Log->append(F1(";lhOn:")).append((unsigned int)lhOn);
	Config.Log->append(F1(";ParentId:")).append((unsigned int)parentId);
	Config.Log->append(F1(" @"));

	Config.Log->Log(level);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}
