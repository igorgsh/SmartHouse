
#include "Relay.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"
#include "SigmaEEPROM.h"

extern Mqtt MqttClient;

void Relay::SetDefault() {

	status = LOW;
}
void Relay::InitUnit() {
	//Loger::Debug("Relay["+String(Id) +"] Init");

	pinMode(Pin, OUTPUT);
	ProcessUnit(status==HIGH ? ACT_ON : ACT_OFF);
	
};

void Relay::RelaySet(bool newStatus)
{
	//Loger::Debug("Relay[" + String(Id) + "]=" + (newStatus == HIGH ? "HIGH" : "LOW") );
	digitalWrite(Pin, (newStatus == HIGH ? lhOn : !lhOn));
	status = newStatus;
	MqttClient.PublishUnit(this);
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

void Relay::UnitLoop() {
//nothing todo
};

bool Relay::Compare(Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::RELAY) return false;
	Relay* tu = (Relay*)u;
	Loger::Debug("Compare Relay:" + String(Id == tu->Id) + ":" + String(Type == tu->Type) + ":" + String(Pin == tu->Pin) + ":" 
		+ String(lhOn == tu->lhOn) + ":" + String(status == tu->status) + "#");
	return (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		lhOn == tu->lhOn &&
		status == tu->status
		);
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


void Relay::ConfigField(JsonObject& jsonList) {
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
	str0 += ";status:";
	str0 += String((unsigned int)status, DEC);
	str0 += ";subscription:";
	str0 += (isSubscribed ? "true" : "false");
	str0 += " @";
	Loger::Log(level, str0);
}

