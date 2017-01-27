// relay.h
#pragma once
#include "Arduino.h"
#include "types.h"

RelayUnit* FindRelay(const char * id);
void RelaySwitch(char *Id);
void RelaySet(char* Id, bool highLow);
void RelaySet(RelayUnit* unit, bool highLow);
void DefaultRelayValue(RelayUnit* relay);


