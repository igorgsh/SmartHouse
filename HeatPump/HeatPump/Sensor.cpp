#include "Sensor.h"

void Sensor::init(String label, int pin, float lowValue, float highValue, int critThreshold) {
	this->label = label;
	this->pin = pin;
	this->lowValue = lowValue;
	this->highValue = highValue;
	this->criticalThreshold = critThreshold;
}


Sensor::Sensor(String label, int pin, float lowValue, float highValue, int critThreshold) {
	init(label, pin, lowValue, highValue, critThreshold);
}

Sensor::Sensor(String label, int pin, int critThreshold) {
	init(label, pin, 0, 0, critThreshold);
}

Sensor::Sensor()
{
}


Sensor::~Sensor()
{
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
