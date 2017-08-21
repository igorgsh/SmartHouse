#include "DeviceItem.h"


DeviceItem::DeviceItem(OutputDevice* device, ScenarioCommand cmd)
{
	this->device = device;
	this->cmd = cmd;
}

DeviceItem::~DeviceItem()
{
}

bool DeviceItem::Run()
{
	device->Command(cmd);
	return true;
}
