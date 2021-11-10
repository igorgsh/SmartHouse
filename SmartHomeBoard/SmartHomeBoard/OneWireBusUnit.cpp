#include "OneWireBusUnit.h"
#include "Loger.h"
#include "Configuration.h"
#include "ext_global.h"


OneWireBus* OneWireBusUnit::FindOneWireBus(byte pin) {
	if (Config.units != NULL) {
		for (int i = 0; i < Config.numberUnits; i++) {
			if (Config.units[i]->Type == UnitType::ONE_WIRE_BUS) {
				if (((OneWireBus*)Config.units[i])->Pin == pin) {
					return (OneWireBus*)Config.units[i];
				}
			}
		}
	}
	return NULL;

}


void OneWireBusUnit::InitUnit() {
	Loger::Debug("Init OneWireBus Unit");
	Loger::Debug("Pin=" + String(Pin));
	//parent = (OneWireBus*)Config.FindUnitByTypeAndPin(UnitType::ONE_WIRE_BUS, Pin);
	parent = (OneWireBus*)FindOneWireBus(Pin);
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
	if (prevCycle + BUS_INTERVAL < millis()) {
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


void const OneWireBusUnit::print(const char* header, DebugLevel level) {
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
	str0 += ";address:";
	for (int i = 0; i < 8; i++) {
		str0 += String(address[i], HEX);
	}
	str0 += " @ ";
	Loger::Log(level, str0);
}

void OneWireBusUnit::FinalInitUnit() {
}


void OneWireBusUnit::ConfigField(const JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("address")) {
		OneWireBus::ConvertStringToAddress(address, jsonList["address"].);
	}
}

