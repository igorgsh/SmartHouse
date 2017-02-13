#include "OneWireBusUnit.h"
#include "Loger.h"
#include "Configuration.h"
#include "ext_global.h"

void OneWireBusUnit::InitUnit() {
	Debug("Init OneWireBus Unit");
	Debug2("Pin=", Pin);
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
		Debug2("prevCycle=", prevCycle);
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


void OneWireBusUnit::print(const char* header, Stream& stream) {
	if (header != NULL) {
		stream.print(header);
	}
	stream.print("Id:");
	stream.print(Id, HEX);
	stream.print(";Type:");
	stream.print(Type, HEX);
	stream.print(";Pin:");
	stream.print(Pin, DEC);
	stream.print(";lhOn:");
	stream.print(lhOn, HEX);
	stream.print(";status:");
	stream.print((unsigned int)status, DEC);
	stream.print(";address:");
	for (int i = 0; i < 8; i++) {
		stream.print(address[i], HEX);
	}
	stream.println(" @");
}

void OneWireBusUnit::FinalInitUnit() {
}
