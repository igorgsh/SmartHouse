#pragma once

#include <Arduino.h>
#include "types.h"

typedef enum {
	BUTTON = 'B',
	RELAY = 'R'
} UnitType;

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
	void FillFrom(Unit* u);
	virtual void InitUnit() =0;
	virtual ~Unit() {};
};

class UnitProto : public Unit {
public:
	void InitUnit() {};

};
