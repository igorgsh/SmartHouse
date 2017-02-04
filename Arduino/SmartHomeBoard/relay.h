// relay.h
#pragma once
#include <Arduino.h>
#include "types.h"
#include "Unit.h"

class Relay : public Unit
{
public:
	void SetDefault();

/*
	RelayUnit* FindRelay(const char * id);
	void RelaySwitch(char *Id);
	void RelaySet(char* Id, bool highLow);
	void RelaySet(RelayUnit* unit, bool highLow);
	void DefaultRelayValue(RelayUnit* relay);
	*/
};




