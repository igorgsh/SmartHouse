#include "Relay.h"
#include "Arduino.h"

Relay::Relay(int pin, bool on) {
	this->pin = pin;
	this->on = on;

	status = !on;
	pinMode(pin, OUTPUT);
	disconnect();
	digitalWrite(pin, status);
}

Relay::~Relay()
{
}


void Relay::connect() {
	if (status != on) {
		status = on;
		digitalWrite(pin, status);
	}
}

void Relay::disconnect() {
	if (status == on) {
		status = !on;
		digitalWrite(pin, status);
	}
}


bool Relay::DeviceCommand(ScenarioCommand cmd)
{
	if (cmd == COMMAND_FORCE_STOP || cmd == COMMAND_STOP) {
		disconnect();
	} else {
		connect();
	}
	return true;
}




