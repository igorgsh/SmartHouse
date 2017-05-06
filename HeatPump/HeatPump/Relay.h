#pragma once
#include "OutputDevice.h"
class Relay : 
	public OutputDevice
{
public:
	Relay(int pin, bool on);
	~Relay();
	void connect();
	void disconnect();
	bool DeviceCommand(ScenarioCommand cmd);

private:
	int pin;
	bool on;
	bool status;
};

