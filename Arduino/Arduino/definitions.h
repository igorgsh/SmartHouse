#pragma once

#include "Arduino.h"
//#include "ext_global.h"

#define NUMBER_OF_BUTTONS 10
#define NUMBER_OF_LIGHTS 10
#define NUMBER_OF_RELAYS 10
#define NUMBER_OF_ACTIONS 10
#define ID_LENGTH 5

#define BUTTON_LONG_PRESS	1500	//after milis button is long pressed
#define BUTTON_SHORT_PRESS	1000	//after milis button is short pressed
#define BUTTON_WRONG_PRESS	200		//ignore click less than this amount of millis 

#define MAX_DIM_VALUE 3 //number of Dimmable levels

typedef enum {
	D_OFF = 0,
	D_FATAL = 1,
	D_ERROR = 2,
	D_WARN = 3,
	D_INFO = 4,
	D_DEBUG = 5,
	D_ALL = 6
} DebugLevel;

extern DebugLevel dLevel;

#define Log2(LEVEL, X, Y) {if (LEVEL <= dLevel) {Serial.println((X),(Y));}};
#define Log(LEVEL, X) {if (LEVEL <= dLevel) {Serial.println((X));}};
#define Debug(X)	Log(D_DEBUG, X)