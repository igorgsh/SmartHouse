#include "Compressor.h"



Compressor::Compressor(int pin, bool on) : OutputDevice(pin, on)
{
}


Compressor::~Compressor()
{
}

void Compressor::StopCompressor()
{
	r->disconnect();
}

void Compressor::StartCompressor()
{
	r->connect();
}

bool Compressor::DeviceCommand(ScenarioCommand cmd)
{
	if (cmd == COMMAND_FORCE_STOP) {
		StopCompressor();
	}
	else if (IsDeviceReady()) {
		if (cmd == COMMAND_STOP) {
			StopCompressor();
			currentCmd = COMMAND_NO_COMMAND;
			status = STATUS_OFF;
		}
		else if (cmd == COMMAND_START) {
			// check statuses of pumps and start it, when it is needed

			for (int i = 1 /* first device is compressor */; i < NUMBER_OF_OUTPUTDEVICES; i++) {
				if (Config.outputDevices[i].status == STATUS_OFF) { //start first pump
					Config.outputDevices[i].Command(COMMAND_START);
				}
			}
			// check statuses and start
			if (Config.outputDevices[1].status == STATUS_ON
				&& Config.outputDevices[2].status == STATUS_ON
				&& Config.outputDevices[3].status == STATUS_ON) {
				StartCompressor();
			}
			else {
				WaitTime(1000); //wait 1 sec until pumps are ready
			}
		}
	}
	return true;
}

