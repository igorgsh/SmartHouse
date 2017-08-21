#pragma once
#include "Relay.h"
#include "OutputDevice.h"

class Compressor :
	public OutputDevice
{
public:
	Compressor(int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Compressor();

private:
	Relay* r;
	bool DeviceCommand(ScenarioCommand cmd);
	void StopCompressor();
	void StartCompressor();

};

