#include "Configuration.h"
#include "Definitions.h"


Configuration::Configuration()
{
	web = new ArduinoServer();
	
}


Configuration::~Configuration()
{
	delete web;
}

void Configuration::begin() {

	web->begin();

	for (int i = 0; i < NUMBER_OF_TEMP; i++) {
		tempSensors[i].begin();
	}
}

bool Configuration::loop(unsigned long counter) {

	bool result = true;
	// check temperature
	for (int i = 0; i < getNumberTemp(); i++) {
		result &= tempSensors[i].loop(counter);
	}
	
	// check contactors
	for (int i = 0; i < getNumberCont(); i++) {
		result &= contacts[i].loop(counter);
	}
	
	result &= web->loop();
	return result;
}

// Функция, возвращающая количество свободного ОЗУ (RAM)
int Configuration::memoryFree()
{
	int freeValue;
	if ((int)__brkval == 0)
		freeValue = ((int)&freeValue) - ((int)&__bss_end);
	else
		freeValue = ((int)&freeValue) - ((int)__brkval);
	return freeValue;
}