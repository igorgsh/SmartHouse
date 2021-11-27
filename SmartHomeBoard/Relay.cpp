
#include "Relay.h"
//#include "process.h"
#include "Configuration.h"
#include "SigmaEEPROM.h"

extern Configuration Config;

void Relay::InitUnit(bool isParent) {
	if (!isParent) {
		pinMode(Pin, OUTPUT);
		//ProcessUnit(status==HIGH ? ACT_ON : ACT_OFF);
	}
};

void Relay::RelaySet(bool newStatus)
{
	if (parentId == 0) {
		digitalWrite(Pin, (newStatus == HIGH ? lhOn : !lhOn));
	}
	else {
		Config.Pass2Parent(parentId, Pin, (newStatus == HIGH ? lhOn : !lhOn));
	}

	status = newStatus;
	Publish(MQTT_RELAYS);
	Config.ProcessAction(Id, newStatus);

}

void Relay::RelaySwitch() {
	RelaySet(!status);
}

void Relay::ProcessUnit(ActionType event) {
	switch (event) {
	case ACT_OFF: {
		RelayOff();
		break;
	}
	case ACT_ON: {
		RelayOn();
		break;
	}
	case ACT_SWITCH: {
		RelaySwitch();
		break;
	}
	default:
		break;
	}
}

void Relay::UnitLoop(unsigned long timePeriod, bool isParent, bool val) {
	//nothing todo
}
void Relay::FinalInitUnit(bool isParent)
{
	Subscribe(MQTT_RELAYS, true);
}


bool Relay::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::RELAY) return false;
	Relay* tu = (Relay*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn &&
		status == tu->status &&
		parentId == tu->parentId 
		);
	return res;
}


void Relay::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read16(addr); //0-1
	Type = SigmaEEPROM::Read8(addr + 2);
	Pin = SigmaEEPROM::Read8(addr + 3);
	lhOn = SigmaEEPROM::Read8(addr + 4);
	status = SigmaEEPROM::Read8(addr + 5);
	parentId = SigmaEEPROM::Read16(addr + 6); //6-7

}

void Relay::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write16(addr, Id); //0-1
	SigmaEEPROM::Write8(addr + 2, Type); 
	SigmaEEPROM::Write8(addr + 3, Pin);
	SigmaEEPROM::Write8(addr + 4, lhOn);
	SigmaEEPROM::Write8(addr + 5, status);
	SigmaEEPROM::Write16(addr + 6, parentId); //6-7
}


void Relay::ConfigField(const JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("lhOn")) {
		lhOn = jsonList["lhOn"];
	}
	//if (jsonList.containsKey("status")) {
	//	status = jsonList["status"];
	//}
	if (jsonList.containsKey("parentId")) {
		parentId = jsonList["parentId"];
	}
}


void const Relay::print(const char* header, DebugLevel level) {
	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F("Id:")).append((unsigned int)Id);
	Config.Log->append(F(";Type:")).append((char)Type);
	Config.Log->append(F(";Pin:")).append((unsigned int)Pin);
	Config.Log->append(F(";lhOn:")).append((unsigned int)lhOn);
	Config.Log->append(F(";status:")).append((unsigned int)status);
	Config.Log->append(F(";ParentId:")).append((unsigned int)parentId);
	Config.Log->append(F(" @"));
	Config.Log->Log(level);
}

