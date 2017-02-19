#pragma once
#include "Sensor.h"
class Contactor :
	public Sensor
{
public:
	Contactor(String label, int pin, bool normal, Relay* r, int critThreshold);
	void begin();
	void requestState();
	bool checkDataReady();
	int getIntValue() { return (currentState ? 1 : 0); };
	float getValue() { return (float)getIntValue(); };
	~Contactor();
	bool getNormal() { return normalState; };
	bool loop(unsigned long counter);

private: 
	bool normalState;
	bool currentState;
};

