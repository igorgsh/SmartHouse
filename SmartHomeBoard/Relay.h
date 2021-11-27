// relay.h
#pragma once
#include <Arduino.h>
#include "Unit.h"

class Relay : public Unit
{
public:
	bool lhOn;

	void InitUnit(bool isParent);
	void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
	void FinalInitUnit(bool isParent);


	void ProcessUnit(ActionType event);
	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 8; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	void RelaySet(bool highLow);
	void RelaySwitch();
	void RelayOn() { RelaySet(HIGH); };
	void RelayOff() { RelaySet(LOW); };

};




