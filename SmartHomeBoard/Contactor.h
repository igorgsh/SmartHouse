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

	byte Pin;
	bool lhOn;



	void SetDefault();
	void InitUnit();

	void ProcessUnit(ActionType event);
	void UnitLoop();
	void FinalInitUnit() {};
	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 7; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	unsigned long startContact;
	byte prevValue;

	void HandleContactor();
	void HandleFinish(int newStatus);
};





#endif

