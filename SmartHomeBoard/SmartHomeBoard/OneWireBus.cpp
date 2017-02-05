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

float OneWireBus::GetTemperature(const byte* address) {
	if (CheckAddress(address)) {
		return sensors->getTempC(address);
	}
	else {
		return -999;
	}
}

bool OneWireBus::CheckAddress(const byte* address) {
	return sensors->isConnected(address);
}

void OneWireBus::InitUnit() {
	Debug("1-Wire init");
	oneWire = new OneWire(Pin);
	sensors = new DallasTemperature(oneWire);
	MqttClient.PublishUnit(this);

}
void OneWireBus::ProcessUnit(byte newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, newStatus, newStatus);
}

void OneWireBus::UnitLoop() {

}
void OneWireBus::FinalInitUnit() {
}
