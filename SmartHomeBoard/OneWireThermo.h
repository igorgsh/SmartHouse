#pragma once
#include <DallasTemperature.h>
#include "Unit.h"
#include "OneWire.h"
#include "OneWireBusUnit.h"

class OneWireThermo :
	public OneWireBusUnit
{
public:


	void UnitLoop();
	void InitUnit();
	void ProcessUnit(ActionType action);
	void HandleData();
	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 11; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);

private:
	
};

