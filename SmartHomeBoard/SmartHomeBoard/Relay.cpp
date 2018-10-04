
#include "Relay.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"
extern Mqtt MqttClient;

void Relay::SetDefault() {

	status = LOW;
}
void Relay::InitUnit() {
	//Loger::Debug("Relay["+String(Id) +"] Init");

	pinMode(Pin, OUTPUT);
	ProcessUnit(status==HIGH ? ACT_ON : ACT_OFF);
	
};

void Relay::RelaySet(bool newStatus)
{
	//Loger::Debug("Relay[" + String(Id) + "]=" + (newStatus == HIGH ? "HIGH" : "LOW") );
	digitalWrite(Pin, (newStatus == HIGH ? lhOn : !lhOn));
	status = newStatus;
	MqttClient.PublishUnit(this);
	Config.ProcessAction(Id, newStatus);
}

void Relay::RelaySwitch() {
	RelaySet(!status);
}

void Relay::ProcessUnit(ActionType event) {
	switch (event) {
	case ACT_OFF: {
		RelayOff();
		break;
	}
	case ACT_ON: {
		RelayOn();
		break;
	}
	case ACT_SWITCH: {
		RelaySwitch();
		break;
	}
	default:
		break;
	}
}

void Relay::UnitLoop() {
//nothing todo
};

