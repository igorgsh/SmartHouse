
#include "Relay.h"
//#include "process.h"
#include "Configuration.h"
#include "SigmaEEPROM.h"

extern Configuration Config;

void Relay::SetDefault() {

	status = LOW;
}
void Relay::InitUnit() {
	pinMode(Pin, OUTPUT);
	ProcessUnit(status==HIGH ? ACT_ON : ACT_OFF);
	
};

void Relay::RelaySet(bool newStatus)
{
	digitalWrite(Pin, (newStatus == HIGH ? lhOn : !lhOn));
	status = newStatus;
	Config.MqttClient->PublishUnit(this);
	Config.ProcessAction(Id, newStatus);
}

void Relay::RelaySwitch() {
	RelaySet(!status);
}

void Relay::ProcessUnit(ActionType event) {
	switch (event) {
	case ACT_OFF: {
		RelayOfF1();
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

void Relay::UnitLoop() {
//nothing todo
};

bool Relay::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::RELAY) return false;
	Relay* tu = (Relay*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn &&
		status == tu->status
		);
	return res;
}


void Relay::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);
	lhOn = SigmaEEPROM::Read8(addr + 3);
	status = SigmaEEPROM::Read8(addr + 4);

}

void Relay::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);
	SigmaEEPROM::Write8(addr + 3, lhOn);
	SigmaEEPROM::Write8(addr + 4, status);

}


void Relay::ConfigField(const JsonObject& jsonList) {
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


void const Relay::print(const char* header, DebugLevel level) {
	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F1("Id:")).append((unsigned int)Id);
	Config.Log->append(F1(";Type:")).append((char)Type);
	Config.Log->append(F1(";Pin:")).append((unsigned int)Pin);
	Config.Log->append(F1(";lhOn:")).append((unsigned int)lhOn);
	Config.Log->append(F1(";status:")).append((unsigned int)status);
	Config.Log->append(F1(" @"));
	Config.Log->Log(level);
}

