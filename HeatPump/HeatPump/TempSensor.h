#pragma once
#include "Sensor.h"
#include "DT.h"

class TempSensor :
	public Sensor
{
public:
	TempSensor(String label, int pin, float lowValue, float highValue);
	TempSensor();
	~TempSensor();
	void begin();
	void requestTemperatures();
	bool checkDataReady();
private:
	DT* dt;
	OneWire* wire;
	void init();
};

