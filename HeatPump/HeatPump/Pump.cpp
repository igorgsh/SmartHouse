#include "Pump.h"



Pump::Pump(int pin, bool on, int minTimeOn, int minTimeOff) 
{
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
}

Pump::~Pump()
{
}

void Pump::StopPump() {
	r->disconnect();
}

void Pump::StartPump() {
	r->connect();
}


bool Pump::DeviceCommand(ScenarioCommand cmd)
{
	if (cmd == COMMAND_FORCE_STOP) {
		StopPump();
		currentCmd = COMMAND_NO_COMMAND;
		status = STATUS_OFF;
	}
	else if (IsDeviceReady()) {
		if (cmd == COMMAND_STOP) {
			StopPump();
			currentCmd = COMMAND_NO_COMMAND;
			status = STATUS_OFF;
		}
		else if (cmd == COMMAND_START) {
			StartPump();
			currentCmd = COMMAND_NO_COMMAND;
			status = STATUS_ON;
		}
	}
	return true;
}

