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

	void InitUnit(bool isParent);
	void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
	void FinalInitUnit(bool isParent) {};

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

	void HandleContactor(unsigned long timePeriod, bool isDirect, bool v);
	void HandleFinish(int newStatus);
};





#endif

