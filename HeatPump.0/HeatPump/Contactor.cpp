#include "Contactor.h"


Contactor::Contactor(String label, int pin, bool normal, ActionScenario* scenario[], int critThreshold) :
	Sensor(label, pin,!normal,!normal,!normal,!normal, scenario, critThreshold) {
	this->normalState = normal;
	this->type = CONTACT;
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
		setError(ErrorCode::CONTACT_ERROR);
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