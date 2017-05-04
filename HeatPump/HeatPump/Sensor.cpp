#include "Sensor.h"

void Sensor::init(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, int critThreshold, ActionScenario* scenario[]) {
	this->label = label;
	this->pin = pin;
	this->alarmLow = alarmLow;
	this->alarmHigh = alarmHigh;
	this->actionLow = startLow;
	this->actionHigh = startHigh;
	this->criticalThreshold = critThreshold;
	if (scenario != NULL) {
		this->scenario[0] = scenario[0];
		this->scenario[1] = scenario[1];
		this->scenario[2] = scenario[2];
		this->scenario[3] = scenario[3];
	}
	else {
		this->scenario[0] = this->scenario[1] = this->scenario[2] = this->scenario[3] = NULL;
	}
}


Sensor::Sensor(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, ActionScenario* scenario[], int critThreshold) {
	init(label, pin, alarmLow, alarmHigh, startLow, startHigh,  critThreshold, scenario);
}
/*
Sensor::Sensor(String label, int pin, Relay* r, int critThreshold) {
	init(label, pin, 0, 0, 0, 0, r, critThreshold);
}
*/
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
		if (actionLow <= currentValue) { // ActionLow
			actionStatus = ACTION_LOW;
		} else if (actionHigh >= currentValue) { // ActionHigh
			actionStatus = ACTION_HIGH;
		}
		else {
			actionStatus = ACTION_NORMAL;
		}
	}
	else { //result is absent
		if (isCritical()) {
			actionStatus = ACTION_NODATA;
		}
		else {
			// Nothing todo. Keep the previous status
		}
	}
	scenario[actionStatus]->Run();
	return res;
}
