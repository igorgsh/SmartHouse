#include "Config.h"



Config::Config()
{
}


Config::~Config()
{
}

void Config::begin() {
	for (int i = 0; i < NUMBER_OF_TEMP; i++) {
		tempSensors[i].begin();
	}
}

bool Config::loop(unsigned long counter) {

	bool result = true;
	// check temperature
	for (int i = 0; i < getNumberTemp(); i++) {
		result &= tempSensors[i].loop(counter);
	}
	
	// check contactors
	for (int i = 0; i < getNumberCont(); i++) {
		result &= contacts[i].loop(counter);
	}
	return result;
}
