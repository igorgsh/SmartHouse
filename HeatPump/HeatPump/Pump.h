#pragma once
#include "Relay.h"
#include "OutputDevice.h"

class Pump : 
	public OutputDevice
{
public:
	Pump(int pin, bool on, int minTimeOn, int minTimeOff);
	~Pump();
private:
	Relay* r;
//	int minTimeOn = 0;
//	int minTimeOff = 0;
	//void Start();
	//void Stop();
	bool DeviceCommand(ScenarioCommand cmd);
	void StopPump();
	void StartPump();
};

