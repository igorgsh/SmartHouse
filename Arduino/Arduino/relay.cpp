
#include "relay.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"

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

void DefaultRelayValue(RelayUnit* relay) {

	relay->status = LOW;
}

void RelaySet(char * id, bool highLow) {
	RelayUnit *unit = FindRelay(id);
	RelaySet(unit, highLow);
}
void RelaySet(RelayUnit* unit, bool highLow)
{
	if (unit != NULL) {
		Debug((highLow == HIGH ? "Relay HIGH" : "Relay LOW"));
		digitalWrite(unit->Pin, (highLow == HIGH ? unit->lhOn : !unit->lhOn));
		unit->status = highLow;
		PublishRelay(*unit);
		ProcessAction(unit->Id, highLow, highLow, !highLow);
	}
}

void RelaySwitch(char * id) {
	RelayUnit *unit = FindRelay(id);

	if (unit != NULL) {
		RelaySet(unit, !(unit->status));
	}
}


