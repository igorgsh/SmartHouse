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
	//parent = (OneWireBus*)Config.FindUnitByTypeAndPin(UnitType::ONE_WIRE_BUS, Pin);
	parent = (OneWireBus*)FindOneWireBus(Pin);
	if (parent == NULL) {
		Log.append(F("Can't find bus for unit: ")).append(Id).Error();
	}
	else {
		if (!parent->CheckAddress(address)) {
			Log.append(F("Unit:")).append(Id).append(F(" is absent on the bus")).Error();
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
		parent->RequestTemperature();
		HandleData();
		prevCycle = millis();
	}
}

void OneWireBusUnit::FillFrom(const Unit* u) {
	Unit::FillFrom(u);
	if (u->Type == UnitType::ONE_WIRE_THERMO) {
		for (int i = 0; i < 8; i++) {
			address[i] = ((OneWireBusUnit*)u)->address[i];
		}
	}
	else {
		Log.append(F("Bad conversion type for unit ")).append(Id).Error();
	}
}


void const OneWireBusUnit::print(const char* header, DebugLevel level) {
	if (header != NULL) {
		Log.append(header);
	}
	Log.append(F("Id:")).append(Id);
	Log.append(F(";Type:")).append(Type);
	Log.append(F(";Pin:")).append(Pin);
	Log.append(F(" @ "));
	Log.Log(level);
}

void OneWireBusUnit::FinalInitUnit() {
}


void OneWireBusUnit::ConfigField(const JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("address")) {
		OneWireBus::ConvertStringToAddress(address, jsonList["address"]);
	}
}

