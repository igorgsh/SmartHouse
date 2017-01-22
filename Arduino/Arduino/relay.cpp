
#include "relay.h"
#include "process.h"
#include "ext_global.h"

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
	//Debug((highLow==HIGH? "Relay HIGH" : "Relay LOW"));
	if (unit != NULL) {
		digitalWrite(unit->Pin, (highLow == HIGH? unit->lhOn : !unit->lhOn));
	}
	ProcessAction(unit->Id, highLow, highLow, unit->status);
	unit->status = highLow;
}

void RelaySwitch(char * id) {
	RelayUnit *unit = FindRelay(id);

	if (unit != NULL) {
		RelaySet(id, !(unit->status));
	}
}


