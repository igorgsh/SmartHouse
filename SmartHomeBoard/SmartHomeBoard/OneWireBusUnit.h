#pragma once
#include "Unit.h"
#include "OneWireBus.h"

class OneWireBusUnit :
	public Unit
{
public:
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
protected:
	unsigned long prevCycle = 0;

};

