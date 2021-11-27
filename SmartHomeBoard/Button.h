#pragma once

#include "definitions.h"
#include "Unit.h"

#define BUTTON_EXTRA_LONG_PRESS	2000	//after milis button is extra long pressed
#define BUTTON_LONG_PRESS	700	//after milis button is long pressed
#define BUTTON_SHORT_PRESS	120	//after milis button is short pressed
#define BUTTON_WRONG_PRESS	100		//ignore click less than this amount of millis 
/*
typedef enum {
	BTN_OFF = 0,
	BTN_ON = 1,
	BTN_LONG = 2,
	BTN_SHORT_LONG = 3,
	BTN_EXTRA_LONG = 4
} ButtonStatus;
*/
class Button : 
	public Unit
{
public:

	bool lhOn;
	
	void InitUnit(bool isParent);
	void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
	void FinalInitUnit(bool isParent);

	void ProcessUnit(ActionType event);
	bool Compare(const Unit* u);
	byte UnitStoredSize() {return 7;}
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	bool isLongMode = false;
	bool isExtraLongMode = false;
	bool isShortMode = false;
	unsigned long startPressing=0;

	void HandleButton(unsigned long timePeriod, bool isParent, bool v);
	void HandleFinish(int newStatus);
};



