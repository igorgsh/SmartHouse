#include "OneWireBusUnit.h"
#include "Loger.h"
#include "Configuration.h"
#include "ext_global.h"

void OneWireBusUnit::InitUnit() {
	Loger::Debug("Init OneWireBus Unit");
	Loger::Debug("Pin=" + String(Pin));
	parent = (OneWireBus*)Config.FindUnitByTypeAndPin(UnitType::ONE_WIRE_BUS, Pin);
	if (parent == NULL) {
		Loger::Error("Can't find bus for unit: " + String(Id));
	}
	else {
		if (!parent->CheckAddress(address)) {
			Loger::Error("Unit:" + String(Id) + " is absent on the bus");
			IsAvailable = false;
		}
		else {
			IsAvailable = true;
			parent->SetResolution(address);
		}
	}
	prevCycle = 0;
}

bool OneWireBusUnit::IsAccessible() {
	return IsAvailable;
}

void OneWireBusUnit::UnitLoop() {
	if (prevCycle + OneWireBus::BUS_INTERVAL < millis()) {
		Loger::Debug("prevCycle=" + String(prevCycle));
		parent->RequestTemperature();
		HandleData();
		prevCycle = millis();
	}
}

void OneWireBusUnit::FillFrom(Unit* u) {
	Unit::FillFrom(u);
	if (u->Type == UnitType::ONE_WIRE_THERMO) {
		for (int i = 0; i < 8; i++) {
			address[i] = ((OneWireBusUnit*)u)->address[i];
		}
	}
	else {
		Loger::Error("Bad conversion type for unit " + String(Id));
	}
}


void OneWireBusUnit::print(const char* header, DebugLevel level) {
	String str0 = "";
	if (header != NULL) {
		str0=header;
	}
	str0 += "Id:";
	str0 += String(Id, HEX);
	str0 += ";Type:";
	str0 += String(Type, HEX);
	str0 += ";Pin:";
	str0 += String(Pin, DEC);
	str0 += ";lhOn:";
	str0 += String(lhOn, HEX);
	str0 += ";status:";
	str0 += String((unsigned int)status, DEC);
	str0 += ";address:";
	for (int i = 0; i < 8; i++) {
		str0 += String(address[i], HEX);
	}
	str0 += " @ ";
	Loger::Log(level, str0);
}

void OneWireBusUnit::FinalInitUnit() {
}
