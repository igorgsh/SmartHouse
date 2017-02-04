
#include "Relay.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"
extern Mqtt MqttClient;

void Relay::SetDefault() {

	status = LOW;
}
void Relay::InitUnit() {
	Debug("Relay Init");

	pinMode(Pin, OUTPUT);
	//digitalWrite(Pin, !lhOn);
	RelaySet(status);
	
};

void Relay::RelaySet(bool highLow)
{
	ProcessUnit(highLow);

}

void Relay::RelaySwitch() {
	RelaySet(!status);
}

void Relay::ProcessUnit(byte newStatus) {
	Debug((newStatus == HIGH ? "Relay HIGH" : "Relay LOW"));
	digitalWrite(Pin, (newStatus == HIGH ? lhOn : !lhOn));
	status = newStatus;
	MqttClient.PublishUnit(this);
	ProcessAction(Id, newStatus, newStatus, !newStatus);

}


