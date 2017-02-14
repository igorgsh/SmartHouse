#include "Config.h"



Config::Config()
{
}


Config::~Config()
{
}

void Config::requestTemperatures() {
	for (int i = 0; i < NUMBER_OF_TEMP; i++) {
		tempSensors[i].requestTemperatures();
	}
}

void Config::begin() {
	for (int i = 0; i < NUMBER_OF_TEMP; i++) {
		tempSensors[i].begin();
	}
}