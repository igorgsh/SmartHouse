#pragma once

#include <Arduino.h>

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
	virtual void UnitLoop() =0;
	virtual void InitUnit() =0;
	virtual void ProcessUnit(byte newStatus) = 0;
	virtual ~Unit() {};
	void print(const char* header, Stream& stream);
};

class UnitProto : public Unit {
public:
	void InitUnit() {};
	void ProcessUnit(byte newStatus) {};
	void UnitLoop() {};

};
