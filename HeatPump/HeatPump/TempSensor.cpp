#include "TempSensor.h"


TempSensor::TempSensor(String label, int pin, float lowValue, float highValue, int critThreshold) : Sensor(label, pin, lowValue, highValue, critThreshold) {
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
		floatValue = dt->getTempCByIndex(0);
		if (floatValue<lowValue) {
			setError(ErrorCode::LOW_VALUE);
			ErrorCounter++;
		} else if (floatValue>highValue) {
			setError(ErrorCode::HIGH_VALUE);
			ErrorCounter++;
		}
		else {
			setError(ErrorCode::NO_ERROR);
			ErrorCounter = 0;
		}

	}
	return ret;
}

void TempSensor::begin() {
	dt->begin();
}
