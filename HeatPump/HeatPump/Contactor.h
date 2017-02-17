#pragma once
#include "Sensor.h"
class Contactor :
	public Sensor
{
public:
	Contactor(String label, int pin, bool normal, int critThreshold);
	void begin();
	void requestState();
	bool checkDataReady();
	int getIntValue() { return (currentState ? 1 : 0); };
	float getValue() { return (float)getIntValue(); };
	Contactor();
	~Contactor();
	bool getNormal() { return normalState; };
private: 
	bool normalState;
	bool currentState;
};

