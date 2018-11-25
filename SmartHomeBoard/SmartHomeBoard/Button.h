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

	byte Pin;
	bool lhOn;


	bool isLongMode = false;
	bool isExtraLongMode = false;
	bool isShortMode = false;
	unsigned long startPressing;
	
	void SetDefault();
	void InitUnit();

	void ProcessUnit(ActionType event);
	void UnitLoop();
	void FinalInitUnit() {};
	bool Compare(Unit* u);
	byte UnitStoredSize() {return 5;}
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	void HandleButton();
	void HandleFinish(int newStatus);
};



