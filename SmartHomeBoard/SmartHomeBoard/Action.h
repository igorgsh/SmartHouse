// action.h
#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "Loger.h"

typedef enum {
	ACT_NO_ACTION = 0,
	ACT_RELAY_ON = 7,
	ACT_RELAY_OFF = 8,
	ACT_RELAY_SWITCH = 9,
	ACT_SENSOR_READY = 10

} ActionType;


class Action {
public:
	uint16_t Id;
	uint16_t originId;
	byte originType;
	byte event;
	uint16_t targetId;
	ActionType targetAction;

	bool compare(Action* a);
	void FillFrom(Action* a);
	void InitAction();
	void print(const char* header, DebugLevel level);
private:
};