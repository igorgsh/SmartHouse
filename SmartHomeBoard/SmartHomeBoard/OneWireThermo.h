#pragma once
#include <DallasTemperature.h>
#include "Unit.h"
#include "OneWire.h"
#include "OneWireBusUnit.h"

class OneWireThermo :
	public OneWireBusUnit
{
public:
	
	void SetDefault();
	//void UnitLoop();
	void InitUnit();
	void ProcessUnit(byte newStatus);
	//void FinalInitUnit();
	void HandleData();

private:
	
};

