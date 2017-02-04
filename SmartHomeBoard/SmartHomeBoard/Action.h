// action.h
#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "types.h"
/*
ActionUnit* FindAction(const char* id);
void DefaultActionValue(const ActionUnit* action);
*/

typedef enum {
	ACT_NO_ACTION = 0,
	ACT_LIGHT_SWITCH = 1,
	ACT_SEND_POST = 2,
	ACT_SEND_LOG = 3,
	ACT_LIGHT_DIM = 4,
	ACT_LIGHT_ON = 5,
	ACT_LIGHT_OFF = 6,
	ACT_RELAY_ON = 7,
	ACT_RELAY_OFF = 8,
	ACT_RELAY_SWITCH = 9

} ActionType;


class Action {
public:
	byte Id;
	byte originId;
	byte originType;
	byte event;
	byte targetId;
	ActionType targetAction;

	bool compare(Action* a);
	void FillFrom(Action* a);
	void InitAction();
	void print(Stream& stream);

private:
};