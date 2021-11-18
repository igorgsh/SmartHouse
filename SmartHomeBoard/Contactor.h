// Contactor.h

#ifndef _CONTACTOR_h
#define _CONTACTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Unit.h"

#define CONTACTOR_SWITCHED_TIME 100

class Contactor:public Unit
{
public:
	bool lhOn;

	void InitUnit();
	void UnitLoop();
	void FinalInitUnit() {};
	void ParentInitUnit();
	void ParentUnitLoop(bool v);
	void ParentFinalInitUnit() {};

	void ProcessUnit(ActionType event);
	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 7; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	unsigned long startContact=0;
	byte prevValue;

	void HandleContactor(bool isDirect, bool v);
	void HandleFinish(int newStatus);
};





#endif

