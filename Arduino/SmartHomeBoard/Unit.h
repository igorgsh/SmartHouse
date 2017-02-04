#pragma once

#include <Arduino.h>
#include "types.h"

class Unit
{
public:
	byte Id;
	byte Type;
	byte Pin;
	bool lhOn;
	int status;
	//static const int sizeOfUnits = 4;

	bool compare(Unit* u);
	void SetDefault() {};
};

