#pragma once
#include <DallasTemperature.h>
#include "Unit.h"
#include "OneWire.h"
#include "OneWireBusUnit.h"

class OneWireThermo :
	public OneWireBusUnit
{
public:


	void SetDefault();
	void UnitLoop();
	void InitUnit();
	void ProcessUnit(ActionType action);
	//void FinalInitUnit();
	void HandleData();
	bool Compare(Unit* u);
	byte UnitStoredSize() { return 11; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);

private:
	
};

