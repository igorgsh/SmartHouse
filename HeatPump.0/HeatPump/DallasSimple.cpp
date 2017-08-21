#include "DallasSimple.h"


float DallasSimple::GetTemperature(DeviceAddress address, bool isSync = true) {
	if (isSync) {
		while (!IsTempReady(address));
	}
	return GetTempC(address);

}


DallasSimple::DallasSimple(OneWire* _wire) 
{
	wire = _wire;
	DeviceAddress deviceAddress;
	// Count number of devices
	numberOfDev = 0;
	wire->reset_search();
	while (wire->search(deviceAddress)) {
		numberOfDev++;
	}
	addresses = (DeviceAddress*)malloc(sizeof(DeviceAddress) * numberOfDev);
	wire->reset_search();
	int i = 0;
	while (wire->search(addresses[i])) {
		i++;
	}
}


DallasSimple::~DallasSimple()
{
	free(addresses);
}

float DallasSimple::GetTempC(DeviceAddress address) {
	wire->reset();
	wire->select(address);
	wire->write(0xBE);
	return wire->read() | (wire->read() << 8); //2 bytes has been read       
}

void DallasSimple::RequestTemperatures() {
	wire->reset();
	wire->write(0xCC);//select all sensors
	wire->write(0x44);// start conversion (no parasite power allowed)
}

void DallasSimple::RequestTemperature(DeviceAddress address) {
	wire->reset();
	wire->select(address);
	wire->write(0x44);// start conversion (no parasite power allowed)
}

bool DallasSimple::IsTempReady(DeviceAddress address) {
	wire->select(address);
	wire->write(0x44);// start conversion (no parasite power allowed)
	wire->reset();

}
