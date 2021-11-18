// 
// 
// 

#include "Contactor.h"

#include "Configuration.h"
//#include "mqtt.h"
#include "Loger.h"
#include "SigmaEEPROM.h"
extern Configuration Config;


void Contactor::ParentInitUnit() {
	prevValue = 0xff;
	startContact = 0;
	status = !lhOn;
}

void Contactor::InitUnit() {
	pinMode(Pin, INPUT);
	digitalWrite(Pin, lhOn? LOW : HIGH);
	ParentInitUnit();
	prevValue = digitalRead(Pin);
}


void Contactor::HandleContactor(bool isDirect, bool v) {

	byte cntValue;

	if (isDirect) {
		cntValue = digitalRead(Pin);
	}
	else {
		cntValue = v;
	}

	if (prevValue != cntValue) { // contactor is starting switch
		if (startContact == 0) {
			startContact = millis();
		}
		else {

			if (startContact + CONTACTOR_SWITCHED_TIME <= millis()) {//contact is long enough
				//Config.Log->append("PPOINT1:v=").append(v).append(" ;prev=").append(prevValue).append(";start=").append(startContact).Debug();
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
	HandleContactor(true, true);
};

void Contactor::ParentUnitLoop(bool v) {
	HandleContactor(false, v);
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
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("lhOn")) {
		lhOn = jsonList["lhOn"];
	}
	if (jsonList.containsKey("parentId")) {
		parentId = jsonList["parentId"];
	}
}


void const Contactor::print(const char* header, DebugLevel level) {
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
}
