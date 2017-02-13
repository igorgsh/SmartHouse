#pragma once
#include "Unit.h"
#include <OneWire.h>
#include "DallasTemperature.h"


class OneWireBus :
	public Unit
{
public:

	void InitUnit();
	void ProcessUnit(int newStatus);
	void UnitLoop();
	void SetDefault();
	void FinalInitUnit();
	float GetTemperature(const DeviceAddress address);
	bool CheckAddress(const DeviceAddress address);
	void SetResolution(const DeviceAddress address);
//	String AddressToString(const DeviceAddress address);
	static const byte BUS_RESOLUTION=9;
	static const int  BUS_INTERVAL = 2000;
	void RequestTemperature();
private:
	OneWire* oneWire;
	int numberThermo = 0;
	DallasTemperature *sensors;
	unsigned long prevCycle = 0;

};

