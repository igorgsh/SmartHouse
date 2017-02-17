#include "Contactor.h"


Contactor::Contactor(String label, int pin, bool normal, int critThreshold) : Sensor(label, pin, critThreshold) {
	this->normalState = normal;
}


Contactor::Contactor()
{
}


Contactor::~Contactor()
{
}

void Contactor::begin() {

}

void Contactor::requestState() {

}

bool Contactor::checkDataReady() {
	currentState = (bool)digitalRead(pin);
//	Serial.println(currentState ? "Button is ON" : "Button is OFF");
	if (currentState != normalState) {
		setError(ErrorCode::SENSOR_DISCONNECTED);
		ErrorCounter++;
	}
	else {
		setError(ErrorCode::NO_ERROR);
		ErrorCounter = 0;
	}

	return true;
}