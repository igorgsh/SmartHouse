// relay.h
#pragma once
#include <Arduino.h>
#include "Unit.h"

class Relay : public Unit
{
public:


	byte Pin;
	bool lhOn;


	void SetDefault();
	void InitUnit();
	void ProcessUnit(ActionType event);
	void UnitLoop();
	void FinalInitUnit() {};
	bool Compare(Unit* u);
	byte UnitStoredSize() { return 5; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	void RelaySet(bool highLow);
	void RelaySwitch();
	void RelayOn() { RelaySet(HIGH); };
	void RelayOff() { RelaySet(LOW); };

};




