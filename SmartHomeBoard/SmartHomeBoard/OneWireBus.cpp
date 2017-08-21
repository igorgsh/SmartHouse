#include "OneWireBus.h"
#include <OneWire.h>
#include "definitions.h"
#include "ext_global.h"
#include "Mqtt.h"
#include "Unit.h"
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
		Debug("Temp is failed");
		return -999;
	}
}

bool OneWireBus::CheckAddress(const DeviceAddress address) {
	return sensors->isConnected(address);
}

void OneWireBus::InitUnit() {
	Debug("1-Wire init");
	oneWire = new OneWire(Pin);
	sensors = new DallasTemperature(oneWire);
	sensors->begin();
	MqttClient.PublishUnit(this);

}
void OneWireBus::ProcessUnit(int newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, newStatus, newStatus);
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