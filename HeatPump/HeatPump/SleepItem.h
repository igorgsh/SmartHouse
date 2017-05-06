#pragma once
#include "ScenarioItem.h"
class SleepItem :
	public ScenarioItem
{
public:
	SleepItem(unsigned long delay);
	~SleepItem();
};

