#include "OneWireThermo.h"
//#include "Loger.h"
#include "OneWireBus.h"
#include "ext_global.h"
#include "Mqtt.h"

extern Mqtt MqttClient;

void OneWireThermo::SetDefault() {
	//nothing todo
}

void OneWireThermo::InitUnit() {
	Loger::Debug(F("Init OneWireThermo Unit"));

	OneWireBusUnit::InitUnit();
	if (!IsAccessible()) {
		Loger::LogMessage = F("Unit: ");
		Loger::LogMessage += Id;
		Loger::LogMessage = F(" Is unavailable on the bus");
		Loger::Error();
	}

}


void OneWireThermo::UnitLoop() {
	//Nothing todo
	OneWireBusUnit::UnitLoop();
}

void OneWireThermo::ProcessUnit(ActionType action) {
// Nothing to do for this kind of unit
}

void OneWireThermo::HandleData() {
	if (IsAccessible()) {
		float t =  parent->GetTemperature(address);
		status = (int)(t*10);
		MqttClient.PublishUnit(this);
		Config.ProcessAction(Id, ACT_SENSOR_READY);
	}
}



bool OneWireThermo::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::ONE_WIRE_THERMO) return false;
	OneWireThermo* tu = (OneWireThermo*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		OneWireBus::CompareDeviceAddress(address, tu->address)
		);
	return res;
}


void OneWireThermo::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);
	
	for (int i = 0; i < 8; i++) {
		address[i] = SigmaEEPROM::Read8(addr + 3 + i);
	}
}

void OneWireThermo::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);

	for (int i = 0; i < 8; i++) {
		SigmaEEPROM::Write8(addr + 3 + i, address[i]);
	}
}




