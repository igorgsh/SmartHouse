#pragma once
#include "Unit.h"
class ShiftRegister :
    public Unit
{
public:
    byte Pin;
    byte Latch;
    byte Clock;
    byte Steps;

	virtual bool Compare(const Unit* u);
	virtual void SetDefault() {};
	virtual void UnitLoop() = 0;
	virtual void InitUnit() = 0;
	virtual byte UnitStoredSize() {return 7;};
	virtual void ReadFromEEPROM(uint16_t addr);
	virtual void WriteToEEPROM(uint16_t addr);
	virtual void FinalInitUnit() {};
	virtual void ProcessUnit(ActionType action) {};
	virtual void const print(const char* header, DebugLevel level);
	virtual void ConfigField(const JsonObject& jsonList) ;


};

