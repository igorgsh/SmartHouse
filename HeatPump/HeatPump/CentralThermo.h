#pragma once
#include "Arduino.h"
#include "TempSensor.h"

class CentralThermo
{
public:
	CentralThermo(TempSensor* t);
	~CentralThermo();
	float getTemp() { return thermometer->getValue(); };
	void setDesiredTemp(float centralTemp);
	float getDesiredTemp() { return centralTemp; };

private:
	TempSensor* thermometer;
	float centralTemp;
};

