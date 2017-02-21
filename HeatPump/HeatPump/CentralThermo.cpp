#include "CentralThermo.h"


void CentralThermo::setDesiredTemp(float temp) {
	//TODO: all coeff should be adjacent
	thermometer->setStartHigh(temp*1.1);
	thermometer->setStartLow(temp*0.7);
	thermometer->setAlarmHigh(temp*1.3);
	thermometer->setAlarmLow(temp*0.2);

	centralTemp = temp;
}

CentralThermo::CentralThermo(TempSensor* t, float desired)
{
	thermometer = t;
	this->setDesiredTemp(desired);
}


CentralThermo::~CentralThermo()
{
}
