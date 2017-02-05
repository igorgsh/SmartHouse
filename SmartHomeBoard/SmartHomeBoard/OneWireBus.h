#pragma once
#include "Unit.h"
#include <OneWire.h>
#include "DallasTemperature.h"

class OneWireBus :
	public Unit
{
public:

	void InitUnit();
	void ProcessUnit(byte newStatus);
	void UnitLoop();
	void SetDefault();
	void FinalInitUnit();
	float GetTemperature(const byte* address);
	bool CheckAddress(const byte* address);
private:
	OneWire* oneWire;
	int numberThermo = 0;
	DallasTemperature *sensors;

};

