#pragma once
#include "OneWire.h"

class DallasSimple
{
public:
	void RequestTemperatures();
	void RequestTemperature(DeviceAddress address);
	bool RequestTemperature(int index) {
		RequestTemperature(addresses[index]);
	}
	bool IsTempReady(DeviceAddress address);
	bool IsTempReady(int index) { return IsTempReady(addresses[index]); }
	int GetDeviceIndex(DeviceAddress addr);
	void GetDeviceAddress(DeviceAddress address, int index);
	float GetTemperature(int index, bool isSync = true) { return GetTemperature(addresses[index], isSync); };
	float GetTemperature(DeviceAddress address, bool isSync = true);
	int GetNumberOfDevices() { return numberOfDev; }
	
	DallasSimple(OneWire* _wire);
	~DallasSimple();
private:
	OneWire* wire;
	DeviceAddress* addresses;
	int numberOfDev;
	float GetTempC(DeviceAddress address);
};

