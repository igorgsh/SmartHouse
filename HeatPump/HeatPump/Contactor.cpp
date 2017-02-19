#include "Contactor.h"


Contactor::Contactor(String label, int pin, bool normal, Relay* r, int critThreshold) : Sensor(label, pin,r, critThreshold) {
	this->normalState = normal;
	pinMode(pin, INPUT);
	digitalWrite(pin, !normal);
}



Contactor::~Contactor()
{
}

void Contactor::begin() {

}

void Contactor::requestState() {

}

bool Contactor::checkDataReady() {
	bool res = true;
	currentState = (bool)digitalRead(pin);
//	Serial.println(currentState ? "Button is ON" : "Button is OFF");
	if (currentState != normalState) {
		setError(ErrorCode::SENSOR_DISCONNECTED);
		ErrorCounter++;
		res = false;
	}
	else {
		setError(ErrorCode::NO_ERROR);
		ErrorCounter = 0;
	}

	return res;
}


bool Contactor::loop(unsigned long counter) {
	bool result = true;
	if (counter % 10 == 0) {// check just first loop 
		result = getData();	//ignore result
	}
	return result;
}