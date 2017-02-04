
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
	Debug((highLow == HIGH ? "Relay HIGH" : "Relay LOW"));
	digitalWrite(Pin, (highLow == HIGH ? lhOn : !lhOn));
	status = highLow;
	MqttClient.PublishUnit(this);
	ProcessAction(Id, highLow, highLow, !highLow);
}

void Relay::RelaySwitch() {
	RelaySet(!status);
}


/*
RelayUnit* FindRelay(const char* id) {

	RelayUnit *unit = NULL;

	for (int i = 0; i < NUMBER_OF_RELAYS && &(Relays[i]) != NULL; i++) {
		if (strcmp(Relays[i].Id, id) == 0) {
			unit = &(Relays[i]);
			break;
		}
		//if (Relays[i].Id[0] != 0) {
		//	break;
		//}
	}
	return unit;
}



*/

