#pragma once

#include "definitions.h"
#include "types.h"
#include "Unit.h"

#define BUTTON_LONG_PRESS	1500	//after milis button is long pressed
#define BUTTON_SHORT_PRESS	1000	//after milis button is short pressed
#define BUTTON_WRONG_PRESS	200		//ignore click less than this amount of millis 

typedef enum {
	BTN_OFF = 0,
	BTN_ON = 1,
	BTN_LONG = 2,
	BTN_SHORT_LONG = 3
} ButtonStatus;

class Button : 
	public Unit
{
public:
	bool isLongMode;
	unsigned long startPressing;
	
	void SetDefault();
	void InitUnit();
	void ProcessButton();

	/*
	void ProcessButton(Button *unit);
	//ButtonUnit* FindButton(const char* id);
	void DefaultButtonValue(Button* btn);
	*/
};



