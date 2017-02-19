#include "TempSensor.h"


TempSensor::TempSensor(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, Relay* r, int critThreshold) 
	: Sensor(label, pin, alarmLow, alarmHigh, startLow,  startHigh, r, critThreshold) {
	init();
}

/*
TempSensor::TempSensor()
{
	init();
}
*/

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
		currentValue = dt->getTempCByIndex(0);
		if (currentValue<alarmLow) {
			setError(ErrorCode::LOW_VALUE);
			ErrorCounter++;
			ret = false;
		} else if (currentValue>alarmHigh) {
			setError(ErrorCode::HIGH_VALUE);
			ErrorCounter++;
			ret = false;
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
	dt->setResolution(DEFAULT_RESOLUTION);
}

bool TempSensor::loop(unsigned long counter) {
	bool result = true;
	if (counter % 10 == 0) {//first loop - request for data
		requestTemperatures();
		result = true;
	}
	else if (counter % 10 == 9) { // the last loop. All sensors which didn't ready marked as Disconnected 
		if (!getData()) {
			setError(ErrorCode::SENSOR_DISCONNECTED);
			ErrorCounter++;
			result = false;
		}
	}
	else { //check/prepare data ready
		result = getData();	
	}
	return result;
}