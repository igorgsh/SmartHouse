#include "Sensor.h"

void Sensor::init(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, Relay* r, int critThreshold) {
	this->label = label;
	this->pin = pin;
	this->alarmLow = alarmLow;
	this->alarmHigh = alarmHigh;
	this->startLow = startLow;
	this->startHigh = startHigh;
	this->criticalThreshold = critThreshold;
	this->r = r;
}


Sensor::Sensor(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, Relay* r, int critThreshold) {
	init(label, pin, alarmLow, alarmHigh, startLow, startHigh,  r, critThreshold);
}

Sensor::Sensor(String label, int pin, Relay* r, int critThreshold) {
	init(label, pin, 0, 0, 0, 0, r, critThreshold);
}

Sensor::Sensor()
{
}


Sensor::~Sensor()
{
}

bool Sensor::getData() {
	bool res;

	res = checkDataReady();
	if (res) { //data is ready
		if (startLow <= currentValue && currentValue >= startHigh) {
			waitingStart = false;
			if (r != NULL) {
				r->connect();
			}
		}
		else {
			waitingStart = true;
		}
	}
	else { //result is absent
		if (isCritical()) {
			waitingStart = true;
			if (r != NULL) {
				r->disconnect();
			}
		}
		else {
			waitingStart = true;
		}
	}
	return res;
}

/*
bool Sensor::checkDataReady() {
	return isDataReady;
}
*/
/*
void Sensor::setDataReady(bool val) {
	isDataReady = val;
}
*/
