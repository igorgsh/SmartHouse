
#include "Relay.h"
//#include "process.h"
#include "Configuration.h"
#include "SigmaEEPROM.h"

extern Configuration Config;

void Relay::InitUnit() {
<<<<<<< HEAD

=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	pinMode(Pin, OUTPUT);
	ProcessUnit(status==HIGH ? ACT_ON : ACT_OFF);
	
};

void Relay::RelaySet(bool newStatus)
{
	if (parentId == 0) {
		digitalWrite(Pin, (newStatus == HIGH ? lhOn : !lhOn));
	}
	else {
		Config.Pass2Parent(parentId, parentPin, (newStatus == HIGH ? lhOn : !lhOn));
	}

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
		status == tu->status &&
		parentId == tu->parentId &&
		parentPin == tu->parentPin
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
	parentPin = SigmaEEPROM::Read8(addr + 8);

}

void Relay::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write16(addr, Id); //0-1
	SigmaEEPROM::Write8(addr + 2, Type); 
	SigmaEEPROM::Write8(addr + 3, Pin);
	SigmaEEPROM::Write8(addr + 4, lhOn);
	SigmaEEPROM::Write8(addr + 5, status);
	SigmaEEPROM::Write16(addr + 6, parentId); //6-7
	SigmaEEPROM::Write8(addr + 8, parentPin);

}


void Relay::ConfigField(const JsonObject& jsonList) {
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
		status = jsonList[F("status")];
	}
	if (jsonList.containsKey("parentId")) {
		parentId = jsonList["parentId"];
	}
	if (jsonList.containsKey("parentPin")) {
		parentPin = jsonList["parentPin"];
	}
}


void const Relay::print(const char* header, DebugLevel level) {
	if (header != NULL) {
		Config.Log->append(header);
	}
<<<<<<< HEAD
	str0 += F("Id:");
	str0 += (unsigned int)Id;
	str0 += F(";Type:");
	str0 += (char)Type;
	str0 += F(";Pin:");
	str0 += (unsigned int)Pin;
	str0 += F(";lhOn:");
	str0 += (unsigned int)lhOn;
	str0 += F(";status:");
	str0 += (unsigned int)status;
	str0 += F(";subscription:");
	str0 += (isSubscribed ? F("true") : F("false"));
	str0 += F(" @");
	Loger::Log(level, str0);
=======
	Config.Log->append(F1("Id:")).append((unsigned int)Id);
	Config.Log->append(F1(";Type:")).append((char)Type);
	Config.Log->append(F1(";Pin:")).append((unsigned int)Pin);
	Config.Log->append(F1(";lhOn:")).append((unsigned int)lhOn);
	Config.Log->append(F1(";status:")).append((unsigned int)status);
	Config.Log->append(F1(";ParentId:")).append((unsigned int)parentId);
	Config.Log->append(F1(";ParentPin:")).append((unsigned int)parentPin);
	Config.Log->append(F1(" @"));
	Config.Log->Log(level);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}

