#pragma once
#include "Sensor.h"
#include "DT.h"

#define DEFAULT_RESOLUTION 10

class TempSensor :
	public Sensor
{
public:
	TempSensor(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, Relay* r, int critThreshold);
	//TempSensor();
	~TempSensor();
	void begin();
	void requestTemperatures();
	bool checkDataReady();
	bool loop(unsigned long counter);
private:
	DT* dt;
	OneWire* wire;
	void init();
};

