// relay.h
#pragma once
#include <Arduino.h>
#include "Unit.h"

class Relay : public Unit
{
public:
	bool lhOn;

	void InitUnit();
	void UnitLoop();
	void FinalInitUnit() {};

	void ParentInitUnit() {};
	void ParentUnitLoop(bool v) { UnitLoop(); };
	void ParentFinalInitUnit() {};


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
	void RelayOfF1() { RelaySet(LOW); };

};




