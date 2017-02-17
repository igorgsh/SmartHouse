#pragma once
#include "Sensor.h"
#include "TempSensor.h"
#include "Contactor.h"

#define NUMBER_OF_TEMP 2
#define NUMBER_OF_CONTACTOR 1

class Config
{
public:
	Config();
	~Config();
	TempSensor tempSensors[NUMBER_OF_TEMP] = {	TempSensor("D1",30,-100.0,22.0, 5), 
												TempSensor("D2",31,-10.0,120.0, 5) };
	Contactor contacts[NUMBER_OF_CONTACTOR] = { Contactor("C1",23,1,5) };
	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	void begin();
};

