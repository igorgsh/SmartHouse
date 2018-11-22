#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "Loger.h"
#include "Action.h"
#include <ArduinoJson.h>


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
//	byte Pin;
//	bool lhOn;
	byte status;
	bool isSubscribed = false;
	//static const int sizeOfUnits = 4;

	virtual bool Compare(Unit* u) =0;
	virtual void FillFrom(Unit* u);
	virtual void SetDefault() =0;
	virtual void UnitLoop() = 0;
	virtual void InitUnit() = 0;
	virtual byte UnitStoredSize() = 0;
	virtual void ReadFromEEPROM(uint16_t addr) = 0;
	virtual void WriteToEEPROM(uint16_t addr) = 0;
	// All units are initialized. Link Bus devices
	virtual void FinalInitUnit() = 0;
	virtual void ProcessUnit(ActionType action) = 0;
	virtual ~Unit() {};
	virtual void const print(const char* header, DebugLevel level);
	virtual void ConfigField(JsonObject& jsonList) = 0;
};

class UnitProto : public Unit {
public:
	void SetDefault() {};
	void InitUnit() {};
	void ProcessUnit(ActionType action) {};
	void UnitLoop() {};
	void FinalInitUnit() {};
	byte UnitStoredSize() {return 0;};
	void ReadFromEEPROM(uint16_t addr) {};
	void WriteToEEPROM(uint16_t addr) {};
	void ConfigField(JsonObject& jsonList) {};
	bool Compare(Unit* u) { return false; };

};
