#pragma once
#include "Sensor.h"
#include "TempSensor.h"

#define NUMBER_OF_TEMP 2

class Config
{
public:
	Config();
	~Config();
	TempSensor tempSensors[NUMBER_OF_TEMP] = {	TempSensor("D1",30,-100.0,20.0), 
												TempSensor("D2",31,-10.0,120.0) };
	int getNumberTemp() { return NUMBER_OF_TEMP; }
	void requestTemperatures();
	void begin();
};

