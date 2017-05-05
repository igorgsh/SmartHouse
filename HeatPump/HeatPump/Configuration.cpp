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
	counter100 = counter;
	if (counter100 % 5 == 0) {
		counter500 = counter100 * 5;
		if (counter500 % 2 == 0) {
			counter1000 = counter500 * 2;
			for (int i = 0; i < NUMBER_OF_OUTPUTDEVICES; i++) {
				if (outputDevices[i].IsDeviceReady() && outputDevices[i].currentCmd != COMMAND_NO_COMMAND) {
					if (outputDevices[i].callBack != NULL) {
						outputDevices[i].callBack();
					}
					else {
						result &= outputDevices[i].DeviceCommand(outputDevices[i].currentCmd);
					}
				}
			}
		}
	}
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