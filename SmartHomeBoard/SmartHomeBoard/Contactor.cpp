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
	//	Loger::Debug("Init Button:id=" + String(Id) + "; Pin=" + String(Pin));
	pinMode(Pin, INPUT);
	digitalWrite(Pin, !lhOn);
	startContact = 0;
	status = !lhOn;
	MqttClient.PublishUnit(this);
}


void Contactor::HandleContactor() {

	byte cntValue;

	cntValue = digitalRead(Pin);
	Loger::Debug("Contactor state:" + String(status) + ":" + String(cntValue));

	if (status != cntValue) { // contactor is starting switch
		//Loger::Debug("Contactor switch:" + String(status) + "->" + String(cntValue));
		//Loger::Debug("Start=" + String(startContact));
		if (startContact == 0) {
			startContact = millis();
		}
		else {
			//Loger::Debug("Point 2:" + String(millis()));
			if (startContact + CONTACTOR_SWITCHED_TIME <= millis()) {//contact is long enough
				//Loger::Debug("Contactor should be switched");
				HandleFinish(cntValue == lhOn ? ACT_ON : ACT_OFF);
				startContact = 0;
			}
		}
	}
	else {
		//Loger::Debug("Point 1");
		startContact = 0;
	}
}

void Contactor::HandleFinish(int newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, newStatus);

}

void Contactor::ProcessUnit(ActionType event) {
	Config.ProcessAction(Id, event);
	MqttClient.PublishUnit(this);

}

void Contactor::UnitLoop() {
	HandleContactor();
};

bool Contactor::Compare(Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::CONTACTOR) return false;
	Contactor *tu = (Contactor*)u;
	bool res = 
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn 
		;
	if (!res) {
		Loger::Debug("Compare Contactor:" + String(Id == tu->Id) + ":" + String(Type == tu->Type) + ":" + String(Pin == tu->Pin) + ":" + String(lhOn == tu->lhOn) + ":" + String(status == tu->status) + "#");
	}
	return res;
}


void Contactor::ReadFromEEPROM(uint16_t addr) {
	//	bool res = true;

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);
	lhOn = SigmaEEPROM::Read8(addr + 3);

}

void Contactor::WriteToEEPROM(uint16_t addr) {
	//bool res = true;

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);
	SigmaEEPROM::Write8(addr + 3, lhOn);
}

void Contactor::ConfigField(JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("lhOn")) {
		lhOn = jsonList["lhOn"];
	}
}


void const Contactor::print(const char* header, DebugLevel level) {
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
