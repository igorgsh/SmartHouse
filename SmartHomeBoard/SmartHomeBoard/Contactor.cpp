// 
// 
// 

#include "Contactor.h"

#include "ext_global.h"
#include "mqtt.h"
//#include "Loger.h"
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
	//	bool res = true;

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
	if (jsonList.containsKey(F("Pin"))) {
		Pin = jsonList[F("Pin")];
	}
	if (jsonList.containsKey(F("lhOn"))) {
		lhOn = jsonList[F("lhOn")];
	}
}


void const Contactor::print(const char* header, DebugLevel level) {
	Loger::LogMessage = F("");

	if (header != NULL) {
		Loger::LogMessage = header;
	}
	Loger::LogMessage += F("Id:");
	Loger::LogMessage+=(unsigned int)Id;
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
}
