#pragma once
#include "Arduino.h"
#include "definitions.h"
/*
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
*/
/*
  typedef enum{
    ACT_NO_ACTION=0,
    ACT_LIGHT_SWITCH=1,
    ACT_SEND_POST=2,
    ACT_SEND_LOG=3,
	ACT_LIGHT_DIM=4,
	ACT_LIGHT_ON=5,
	ACT_LIGHT_OFF = 6,
	ACT_RELAY_ON = 7,
	ACT_RELAY_OFF = 8,
	ACT_RELAY_SWITCH = 9

  } Action;

 
  typedef struct {
	  char Id[ID_LENGTH];
	  char originId[ID_LENGTH];
      Action action;
      byte event;
      char targetId[ID_LENGTH]; 
    } ActionUnit;
	*/

  typedef enum {
    BTN_OFF=0,
    BTN_ON=1,
    BTN_LONG=2,
	BTN_SHORT_LONG=3
  } ButtonStatus;
 
  typedef enum {
	  BUTTON='B',
	  RELAY='R'
  } UnitType;
/*
  typedef struct {
	  byte id;
	  UnitType type;
	  byte bus;
	  byte pin;
	  bool lhOn;
	  ButtonStatus status;
	  bool isLongMode;
	  unsigned long startPressing;
  } Unit;
  */
  
  /*
  typedef struct {
    char Id[ID_LENGTH];
    byte Pin;
	bool lhOn;
	ButtonStatus status;
	bool isLongMode;
    unsigned long startPressing;
  } ButtonUnit;

  typedef struct {
    char Id[ID_LENGTH];
    byte Pin;
	byte dimValue;
	bool status;
  } LightUnit;

  typedef struct {
	  char Id[ID_LENGTH];
	  byte Pin;
	  bool lhOn;
	  bool status;
  } RelayUnit;
  */
  /*
  typedef enum {
	  GET = 0,
	  POST = 1,
	  DELETE = 2
  } RequestType;

  typedef struct {
	  RequestType type;
	  String URL;
	  String host;
	  String body;
  } HttpRequest;
  */
