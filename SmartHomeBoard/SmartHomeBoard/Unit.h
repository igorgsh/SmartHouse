#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "Loger.h"
#include "Action.h"

typedef enum {
	BUTTON = 'B',
	RELAY = 'R',
	ONE_WIRE_BUS = '1',
	ONE_WIRE_THERMO = 'T',
	POWER_METER = 'P',
	VIRTUAL_BUTTON = 'b'
} UnitType;

class Unit
{
public:
	uint16_t Id;
	byte Type;
	byte Pin;
	bool lhOn;
	int status;
	bool isSubscribed = false;
	//static const int sizeOfUnits = 4;

//	bool compare(Unit* u);
	virtual void FillFrom(Unit* u);
	virtual void SetDefault() =0;
	virtual void UnitLoop() = 0;
	virtual void InitUnit() = 0;
	// All units are initialized. Link Bus devices
	virtual void FinalInitUnit() = 0;
	virtual void ProcessUnit(ActionType action) = 0;
	virtual ~Unit() {};
	virtual void const print(const char* header, DebugLevel level);
};

class UnitProto : public Unit {
public:
	void SetDefault() {};
	void InitUnit() {};
	void ProcessUnit(ActionType action) {};
	void UnitLoop() {};
	void FinalInitUnit() {};
};
