#pragma once
#include "Unit.h"
#include "OneWireBus.h"

class OneWireBusUnit :
	public Unit
{
public:
	
	byte Pin;

	DeviceAddress address;
	bool IsAvailable = false;
	OneWireBus *parent;

	void InitUnit();
	void UnitLoop();
	void FinalInitUnit();

	void ParentInitUnit() {};
	void ParentUnitLoop(bool v) {};
	void ParentFinalInitUnit() {};

	bool IsAccessible();
	virtual void HandleData()=0;
	void FillFrom(const Unit* u);
	void const print(const char* header, DebugLevel level) override;
	void ConfigField(const JsonObject& jsonList) override;


protected:
	unsigned long prevCycle = 0;
	OneWireBus* FindOneWireBus(byte pin);


};

