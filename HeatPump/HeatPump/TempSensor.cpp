#include "TempSensor.h"


TempSensor::TempSensor(String label, int pin, float lowValue, float highValue) : Sensor(label, pin, lowValue, highValue) {
	init();
}


TempSensor::TempSensor()
{
	init();
}


TempSensor::~TempSensor()
{
	delete dt;
	delete wire;
}

void TempSensor::init() {

	wire = new OneWire(pin);
	dt = new DT(wire);

}

void TempSensor::requestTemperatures() {
	dt->requestTemperatures();
}

bool TempSensor::checkDataReady() {
	bool ret = dt->isConversionAvailable(0);
	if (ret) {
		value = dt->getTempCByIndex(0);
	}
	return ret;
}

void TempSensor::begin() {
	dt->begin();
}
