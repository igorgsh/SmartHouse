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
	Loger::Debug("Init OneWireThermo Unit");

	OneWireBusUnit::InitUnit();
	Loger::Debug("IsAccessible=" + String( IsAccessible()));
	if (!IsAccessible()) {
		Loger::Error("Unit: " + String(Id) + " Is unavailable on the bus");
	}

}


void OneWireThermo::UnitLoop() {
	//Nothing todo
	//Debug("Thermo Loop");
	OneWireBusUnit::UnitLoop();
}

void OneWireThermo::ProcessUnit(int newStatus) {
	status = newStatus;
	MqttClient.PublishUnit(this);
}

void OneWireThermo::HandleData() {
	Loger::Debug("Get Temperature");
	if (IsAccessible()) {
		float t =  parent->GetTemperature(address);
		Loger::Debug("Temperature=" + String(t));
		Loger::Debug("Int temp=" + String((int)(t * 10)));
		ProcessUnit((int)(t*10));
		Config.ProcessAction(Id, ACT_SENSOR_READY , status);
	}
}

