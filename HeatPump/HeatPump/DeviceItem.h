#pragma once
#include "ScenarioItem.h"
#include "OutputDevice.h"

class DeviceItem :
	public ScenarioItem
{
public:
	DeviceItem(OutputDevice* device, ScenarioCommand cmd);
	~DeviceItem();

	bool Run();
private:
	OutputDevice* device;
	ScenarioCommand cmd;

};

