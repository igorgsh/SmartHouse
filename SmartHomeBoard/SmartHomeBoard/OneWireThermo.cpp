#include "OneWireThermo.h"
#include "Loger.h"
#include "OneWireBus.h"
#include "Loger.h"
#include "ext_global.h"
#include "Mqtt.h"

extern Mqtt MqttClient;

void OneWireThermo::SetDefault() {
	//nothing todo
}

void OneWireThermo::InitUnit() {
	Debug("Init OneWireThermo Unit");

	OneWireBusUnit::InitUnit();
	Debug2("IsAccessible=", IsAccessible());
	if (!IsAccessible()) {
		Loger::Error("Unit: " + String(Id) + " Is unavailable on the bus");
	}
}

/*
void OneWireThermo::UnitLoop() {
	//Nothing todo
}
*/
void OneWireThermo::ProcessUnit(byte newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
}
/*
void OneWireThermo::FinalInitUnit() {

};
*/
void OneWireThermo::HandleData() {
	if (IsAccessible()) {
		float t =  parent->GetTemperature(address);
		ProcessUnit((int)(t*10));
		Config.ProcessAction(Id, ACT_SENSOR_READY , status);
	}
}