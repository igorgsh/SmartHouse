#pragma once
#include "ScenarioItem.h"
#include "Relay.h"

class PumpItem :
	public ScenarioItem
{
public:
	PumpItem(Relay* r);
	~PumpItem();
private:
	Relay* r;
};

