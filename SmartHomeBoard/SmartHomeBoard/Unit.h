#pragma once

#include <Arduino.h>

typedef enum {
	BUTTON = 'B',
	RELAY = 'R',
	ONE_WIRE_BUS = '1',
	ONE_WIRE_THERMO = 'T'
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
	virtual void FillFrom(Unit* u);
	virtual void SetDefault() =0;
	virtual void UnitLoop() = 0;
	virtual void InitUnit() = 0;
	// All units are initialized. Link Bus devices
	virtual void FinalInitUnit() = 0;
	virtual void ProcessUnit(int newStatus) = 0;
	virtual ~Unit() {};
	virtual void print(const char* header, Stream& stream);
};

class UnitProto : public Unit {
public:
	void SetDefault() {};
	void InitUnit() {};
	void ProcessUnit(int newStatus) {};
	void UnitLoop() {};
	void FinalInitUnit() {};
};
