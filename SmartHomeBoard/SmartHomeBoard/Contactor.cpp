// 
// 
// 

#include "Contactor.h"

<<<<<<< HEAD
#include "ext_global.h"
#include "mqtt.h"
//#include "Loger.h"
=======
#include "Configuration.h"
//#include "mqtt.h"
#include "Loger.h"
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
#include "SigmaEEPROM.h"
extern Configuration Config;

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
	Config.MqttClient->PublishUnit(this);
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
	Config.MqttClient->PublishUnit(this);
	Config.ProcessAction(Id, status);

}

void Contactor::ProcessUnit(ActionType event) {
	Config.ProcessAction(Id, event);
	Config.MqttClient->PublishUnit(this);

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
		lhOn == tu->lhOn && 
		parentId == tu->parentId
		;
	return res;
}


void Contactor::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read16(addr); //0-1
	Type = SigmaEEPROM::Read8(addr + 2);
	Pin = SigmaEEPROM::Read8(addr + 3);
	lhOn = SigmaEEPROM::Read8(addr + 4);
	parentId = SigmaEEPROM::Read16(addr + 5); //5-6
}

void Contactor::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write16(addr, Id); //0-1
	SigmaEEPROM::Write8(addr + 2, Type);
	SigmaEEPROM::Write8(addr + 3, Pin);
	SigmaEEPROM::Write8(addr + 4, lhOn);
	SigmaEEPROM::Write16(addr + 5 , parentId); //5-6

}

void Contactor::ConfigField(const JsonObject& jsonList) {
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
	if (jsonList.containsKey("parentId")) {
		parentId = jsonList["parentId"];
	}
}


void const Contactor::print(const char* header, DebugLevel level) {
<<<<<<< HEAD
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
