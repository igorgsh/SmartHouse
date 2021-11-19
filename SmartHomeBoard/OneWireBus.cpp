#include "OneWireBus.h"
#include <OneWire.h>
#include "definitions.h"
//#include "ext_global.h"
//#include "Mqtt.h"
#include "Unit.h"
#include "Loger.h"
#include <DallasTemperature.h>
#include "Configuration.h"

extern Configuration Config;


float OneWireBus::GetTemperature(const DeviceAddress address) {
	if (CheckAddress(address)) {
		return sensors->getTempC(address);
	}
	else {

		Config.Log->append(F1("Temp is failed: ")).append(Id).Error();
		return -999;
	}
}

bool OneWireBus::CheckAddress(const DeviceAddress address) {
	return sensors->isConnected(address);
}



void OneWireBus::InitUnit(bool isParent) {
	if (!isParent) {
		oneWire = new OneWire(Pin);
		sensors = new DallasTemperature(oneWire);
		sensors->begin();
		Config.MqttClient->PublishUnit(this);
		Config.ProcessAction(Id, status);
	}
}

void OneWireBus::ProcessUnit(ActionType action) {
	status = action;
	Config.MqttClient->PublishUnit(this);
	Config.ProcessAction(Id, action);
}

void OneWireBus::UnitLoop() {
	//Nothing todo

}
void OneWireBus::FinalInitUnit() {
}

void OneWireBus::SetResolution(const DeviceAddress address) {
	sensors->setResolution(address, BUS_RESOLUTION);
}

void OneWireBus::RequestTemperature() {
	sensors->requestTemperatures();
}

bool OneWireBus::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::ONE_WIRE_BUS) return false;
	OneWireBus* tu = (OneWireBus*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin
		);
	return res;
}


void OneWireBus::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);

}

void OneWireBus::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);
}


void OneWireBus::ConfigField(const JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
}


void OneWireBus::ConvertStringToAddress(DeviceAddress address, String addrStr) {
	for (int i = 0, j = 0; i < 16; i += 2, j++) {
		unsigned long l = addrStr[i];
		strtoul(addrStr.substring(i, i + 2).c_str(), NULL, 16);
		address[j] = l;
	}
}

/*
String OneWireBus::ConvertAddressToString(const DeviceAddress address) {
	String str0 = "";

	for (int i = 0; i < 16; i++) {
		str0 += String(address[i], HEX);
	}
	return str0;
}
*/

bool OneWireBus::CompareDeviceAddress(DeviceAddress a0, DeviceAddress a1) {
	bool res = false;

	for (int i = 0; !res && i < 8; i++) {
		res &= (a0[i] == a1[i]);
	}
	return res;
}


void const OneWireBus::print(const char* header, DebugLevel level) {
	
	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F1("Id:")).append((unsigned int)Id);
	Config.Log->append(F1(";Type:")).append((char)Type);
	Config.Log->append(F1(";Pin:")).append((unsigned int)Pin);
	Config.Log->append(F1(" @"));
	Config.Log->Log(level);
}
