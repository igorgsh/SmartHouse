#pragma once
#include "Relay.h"
#include "OutputDevice.h"

class Compressor :
	public OutputDevice
{
public:
	Compressor(int pin, bool on);
	~Compressor();

private:
	Relay* r;
	bool DeviceCommand(ScenarioCommand cmd);
	void StopCompressor();
	void StartCompressor();

};

