#include "OneWireBusUnit.h"
//#include "Loger.h"
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
	Loger::Debug(F("Init OneWireBus Unit"));
	parent = (OneWireBus*)FindOneWireBus(Pin);
	if (parent == NULL) {
		Loger::LogMessage = F("Can't find bus for unit: ");
		Loger::LogMessage += Id;
		Loger::Error();
	}
	else {
		if (!parent->CheckAddress(address)) {
			Loger::LogMessage = F("Unit:");
			Loger::LogMessage += Id;
			Loger::LogMessage = F(" is absent on the bus");
			Loger::Error();
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
		Loger::LogMessage = F("Bad conversion type for unit ");
		Loger::LogMessage += Id;

		Loger::Error();
	}
}


void const OneWireBusUnit::print(const char* header, DebugLevel level) {
	String str0 = "";
	if (header != NULL) {
		str0=header;
	}
	str0 += F("Id:");
	str0 += Id;
	str0 += F(";Type:");
	str0 += (char)Type;
	str0 += F(";Pin:");
	str0 += Pin;
	str0 += F(";address:");
	OneWireBus::ConvertAddressToString(address, str0);
	str0 += F(" @ ");
	Loger::Log(level, str0);
}

void OneWireBusUnit::FinalInitUnit() {
}


void OneWireBusUnit::ConfigField(const JsonObject& jsonList) {
	if (jsonList.containsKey(F("Pin"))) {
		Pin = jsonList[F("Pin")];
	}
	if (jsonList.containsKey(F("address"))) {
		OneWireBus::ConvertStringToAddress(address, jsonList[F("address")]);
	}
}

