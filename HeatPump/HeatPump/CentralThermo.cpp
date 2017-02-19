#include "CentralThermo.h"


void CentralThermo::setDesiredTemp(float temp) {
	//TODO: all coeff should be adjacent
	thermometer->setStartHigh(temp*1.1);
	thermometer->setStartLow(temp*0.7);
	thermometer->setAlarmHigh(temp*1.3);
	thermometer->setAlarmLow(temp*0.2);
}

CentralThermo::CentralThermo(TempSensor* t)
{
	thermometer = t;

}


CentralThermo::~CentralThermo()
{
}
