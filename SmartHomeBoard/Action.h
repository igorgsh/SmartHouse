// action.h
#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "Loger.h"



class Action {
public:
	uint16_t Id;
	uint16_t originId;
	UnitType originType;
	byte event;
	uint16_t targetId;
	ActionType targetAction;
	UnitType targetType;

	bool Compare(const Action* a);
	void FillFrom(const Action* a);
	void InitAction();
	void print(const char* header, DebugLevel level);
	void ReadFromEEPROM(uint16_t curPtr);
	void WriteToEEPROM(uint16_t addr);
	int ActionStoredSize() { return 7; };

private:
};