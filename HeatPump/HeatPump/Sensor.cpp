#include "Sensor.h"


Sensor::Sensor(String label, int pin, float lowValue, float highValue) {
	this->label = label;
	this->pin = pin;
	this->lowValue = lowValue;
	this->highValue = highValue;
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
