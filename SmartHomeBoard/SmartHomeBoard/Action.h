// action.h
#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "Loger.h"

typedef enum {
	ACT_OFF = 0, //Off for any device
	ACT_ON = 1, //On for any device
	ACT_SWITCH = 3, //Switch device
	ACT_LONG = 2, //Long (for button)
	ACT_SHORT_LONG = 5, //Button. Is pressed not released yet. And pressed time less than long
	ACT_EXTRA_LONG = 4, //Button. Extra long
	ACT_SENSOR_READY = 10 //Sensor ready

} ActionType;


class Action {
public:
	uint16_t Id;
	uint16_t originId;
	byte originType;
	byte event;
	uint16_t targetId;
	ActionType targetAction;
	byte targetType;

	bool compare(Action* a);
	void FillFrom(Action* a);
	void InitAction();
	void print(const char* header, DebugLevel level);
private:
};