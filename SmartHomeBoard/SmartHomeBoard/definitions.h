#pragma once

#include <Arduino.h>
#include <MemoryFree.h>


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
<<<<<<< HEAD
//#define MEMFREE Loger::Debug("Memory=" + String(memoryFree(),DEC));
=======
#define MEMFREE(N) Config.Log->append(("Memory_")).append(N).append("=").append(freeMemory()).Debug();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516


typedef enum {
	BUTTON			= 'B',
	RELAY			= 'R',
	ONE_WIRE_BUS	= '1',
	ONE_WIRE_THERMO = 'T',
	POWER_METER		= 'P',
	VIRTUAL_BUTTON	= 'b',
	CONTACTOR		= 'C',
	SHIFT_OUT		='H',
	SHIFT_IN		='h'
} UnitType;


typedef enum {
	ACT_OFF = 0, //Off for any device
	ACT_ON = 1, //On for any device
	ACT_SWITCH = 3, //Switch device
	ACT_LONG = 2, //Long (for button)
	ACT_SHORT_LONG = 5, //Button. Is pressed not released yet. And pressed time less than long
	ACT_EXTRA_LONG = 4, //Button. Extra long
	ACT_SENSOR_READY = 10 //Sensor ready

} ActionType;

#define F1	F