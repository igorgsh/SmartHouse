// 
// 
// 

#include "Contactor.h"

#include "Configuration.h"
//#include "mqtt.h"
#include "Loger.h"
#include "SigmaEEPROM.h"
extern Configuration Config;



void Contactor::InitUnit(bool isParent) {
	if (!isParent) {
		pinMode(Pin, INPUT);
		digitalWrite(Pin, lhOn ? LOW : HIGH);
	}
	prevValue = 0xff;
	startContact = 0;
	status = !lhOn;
	//Config.Log->append("SIR. Prev=").append(prevValue).Debug();
}


void Contactor::HandleContactor(unsigned long timePeriod, bool isParent, bool v) {

	byte cntValue;

	if (!isParent) {
		cntValue = digitalRead(Pin);
	}
	else {
		cntValue = v;
	}
	//Config.Log->append("SIR: Parent=").append(isParent).append("; v=").append(v).append("; prev=").append(prevValue).Debug();
	if (prevValue != cntValue) { // contactor is starting switch
		unsigned long now = millis();
		//Config.Log->append("SIR: Parent=").append(isParent).append("; v=").append(v)
		//	.append("; start=").append(startContact).append("; delta=").append(now-startContact).Debug();
		if (startContact == 0) {
			startContact = now;
		}
		else {

			if (startContact + CONTACTOR_SWITCHED_TIME <= now) {//contact is long enough
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
	Publish(MQTT_CONTACTOR);
	Config.ProcessAction(Id, status);

}

void Contactor::ProcessUnit(ActionType event) {
	Config.ProcessAction(Id, event);
	Publish(MQTT_CONTACTOR);
}

void Contactor::UnitLoop(unsigned long timePeriod, bool isParent, bool val) {
	if (parentId == 0
		|| (parentId != 0 && isParent)) {
		HandleContactor(timePeriod, isParent, val);
	}
}
void Contactor::FinalInitUnit(bool isParent)
{
	// No Subscription
	// No Values request
}
;



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
	Config.Log->append(F("Id:")).append((unsigned int)Id);
	Config.Log->append(F(";Type:")).append((char)Type);
	Config.Log->append(F(";Pin:")).append((unsigned int)Pin);
	Config.Log->append(F(";lhOn:")).append((unsigned int)lhOn);
	Config.Log->append(F(";ParentId:")).append((unsigned int)parentId);
	Config.Log->append(F(" @"));
	Config.Log->Log(level);
}
