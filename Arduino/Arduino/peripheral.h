#pragma once
#include "Arduino.h"
#include "definitions.h"

  typedef enum {
    UT_NOPE=0, 
    UT_BUTTON=1,
    UT_SENSOR=2,
    UT_LIGHT=3,
    UT_PIN=4
  } UnitType;

  typedef enum{
    AD_NOPE=0,
    AD_ANALOG=1,
    AD_DIGITAL=2
  } AnalogDigital;

  typedef enum{
    ACT_NO_ACTION=0,
    ACT_LIGHT_SWITCH=1,
    ACT_SEND_POST=2,
    ACT_SEND_LOG=3,
	ACT_LIGHT_DIM=4,
	ACT_LIGHT_ON=5,
	ACT_LIGHT_OFF = 6

  } Action;

 
  typedef struct {
      Action action;
      byte condition;
      char Id[ID_LENGTH]; 
    } ActionDescription;

/*
  typedef struct {
    char Id[ID_LENGTH];
    byte Type;
    bool io;
    byte AdType;
    byte Pin;
    double Value;
    ActionDescription Action[NUMBER_OF_ACTIONS];
  } PeripheralUnit;
*/

  typedef enum {
    BTN_OFF=0,
    BTN_ON=1,
    BTN_LONG=2,
	BTN_SHORT_LONG=3
  } ButtonStatus;

  typedef struct {
    char Id[ID_LENGTH];
    byte Pin;
    ActionDescription Actions[NUMBER_OF_ACTIONS];
	bool isLongMode;
    unsigned long startPressing;
  } ButtonUnit;

  typedef struct {
    char Id[ID_LENGTH];
    byte Pin;
	bool status;
	byte dimValue;
    ActionDescription Actions[NUMBER_OF_ACTIONS];
  } LightUnit;
