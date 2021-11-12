// 
// 
// 

#include "Contactor.h"

#include "ext_global.h"
#include "mqtt.h"
#include "Loger.h"
#include "SigmaEEPROM.h"

extern Mqtt MqttClient;

void Contactor::SetDefault() {
	startContact = 0;
	status = ActionType::ACT_OFF;
}

void Contactor::InitUnit() {
	pinMode(Pin, INPUT);
	digitalWrite(Pin, lhOn? LOW : HIGH);
	startContact = 0;
	prevValue = digitalRead(Pin);
	status = !lhOn;
	MqttClient.PublishUnit(this);
}


void Contactor::HandleContactor() {

	byte cntValue;

	cntValue = digitalRead(Pin);

	if (prevValue != cntValue) { // contactor is starting switch
		if (startContact == 0) {
			startContact = millis();
		}
		else {
			if (startContact + CONTACTOR_SWITCHED_TIME <= millis()) {//contact is long enough
				HandleFinish(cntValue == lhOn ? ACT_ON : ACT_OFF);
				startContact = 0;
				prevValue = cntValue;
			}
		}
	}
}

void Contactor::HandleFinish(int newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, status);

}

void Contactor::ProcessUnit(ActionType event) {
	Config.ProcessAction(Id, event);
	MqttClient.PublishUnit(this);

}

void Contactor::UnitLoop() {
	HandleContactor();
};

bool Contactor::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::CONTACTOR) return false;
	Contactor *tu = (Contactor*)u;
	bool res = 
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn 
		;
	return res;
}


void Contactor::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);
	lhOn = SigmaEEPROM::Read8(addr + 3);

}

void Contactor::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);
	SigmaEEPROM::Write8(addr + 3, lhOn);
}

void Contactor::ConfigField(const JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("lhOn")) {
		lhOn = jsonList["lhOn"];
	}
}


void const Contactor::print(const char* header, DebugLevel level) {
	if (header != NULL) {
		Log.append(header);
	}
	Log.append(F1("Id:")).append((unsigned int)Id);
	Log.append(F1(";Type:")).append((char)Type);
	Log.append(F1(";Pin:")).append((unsigned int)Pin);
	Log.append(F1(";lhOn:")).append((unsigned int)lhOn);
//	Log.append(F1(";subscription:")).append(isSubscribed ? "true" : "false");
	Log.append(F1(" @"));
	Log.Log(level);
}
