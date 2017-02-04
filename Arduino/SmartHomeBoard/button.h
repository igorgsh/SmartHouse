#pragma once

#include "definitions.h"
#include "types.h"
#include "Unit.h"

class Button : 
	public Unit
{
public:
	bool isLongMode;
	unsigned long startPressing;
	
	void SetDefault();

	/*
	void ProcessButton(Button *unit);
	//ButtonUnit* FindButton(const char* id);
	void DefaultButtonValue(Button* btn);
	*/
};



