#include "OneWireBus.h"
#include <OneWire.h>
#include "definitions.h"
#include "ext_global.h"
#include "Mqtt.h"
#include "Unit.h"
#include "Loger.h"
#include <DallasTemperature.h>
//#include <vector>
//#include <StandardCplusplus-master\vector>
extern Mqtt MqttClient;

void OneWireBus::SetDefault() {

}

float OneWireBus::GetTemperature(const DeviceAddress address) {
	if (CheckAddress(address)) {
		return sensors->getTempC(address);
	}
	else {

		Loger::Error("Temp is failed: " + ConvertAddressToString(address));
		return -999;
	}
}

bool OneWireBus::CheckAddress(const DeviceAddress address) {
	return sensors->isConnected(address);
}

void OneWireBus::InitUnit() {
	Loger::Debug("1-Wire init");
	oneWire = new OneWire(Pin);
	sensors = new DallasTemperature(oneWire);
	sensors->begin();
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, status);

}
void OneWireBus::ProcessUnit(ActionType action) {
	status = action;
	MqttClient.PublishUnit(this);
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

bool OneWireBus::Compare(Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::ONE_WIRE_BUS) return false;
	OneWireBus* tu = (OneWireBus*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin
		);
	if (!res) {
		Loger::Debug("Compare 1-Wire bus:" + String(Id == tu->Id) + ":" + String(Type == tu->Type) + ":" + String(Pin == tu->Pin) + "#");
	}
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


void OneWireBus::ConfigField(JsonObject& jsonList) {
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
}


void OneWireBus::ConvertStringToAddress(DeviceAddress address, const String addrStr) {
	for (int i = 0, j = 0; i < 16; i += 2, j++) {
		unsigned long l = strtoul(addrStr.substring(i, i + 2).c_str(), NULL, 16);
		address[j] = l;
	}
}


String OneWireBus::ConvertAddressToString(const DeviceAddress address) {
	String str0 = "";

	for (int i = 0; i < 16; i++) {
		str0 += String(address[i], HEX);
	}
	return str0;
}


bool OneWireBus::CompareDeviceAddress(DeviceAddress a0, DeviceAddress a1) {
	bool res = false;

	for (int i = 0; !res && i < 8; i++) {
		res &= (a0[i] == a1[i]);
	}
	return res;
}


void const OneWireBus::print(const char* header, DebugLevel level) {
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
	str0 += ";subscription:";
	str0 += (isSubscribed ? "true" : "false");
	str0 += " @";
	Loger::Log(level, str0);
}
