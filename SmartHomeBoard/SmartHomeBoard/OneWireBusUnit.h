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
	bool IsAccessible();
	virtual void HandleData()=0;
	void UnitLoop();
	void FillFrom(Unit* u);
	void FinalInitUnit();
	void const print(const char* header, DebugLevel level) override;
	void ConfigField(JsonObject& jsonList) override;


protected:
	unsigned long prevCycle = 0;
	OneWireBus* FindOneWireBus(byte pin);


};

