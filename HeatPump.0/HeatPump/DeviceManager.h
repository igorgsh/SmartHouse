#pragma once
#include "OutputDevice.h"
#include "Compressor.h"
#include "Pump.h"

/*
*/
class DeviceManager :
	public OutputDevice
{
public:
	DeviceManager();
	~DeviceManager();

	Compressor compressor = Compressor(20, LOW, 3000, 3000);
	Pump pumpGeo = Pump(21, LOW, 3000, 3000);
	Pump pumpContour1 = Pump(22, LOW, 3000, 3000);
	Pump pumpContour2 = Pump(23, LOW, 3000, 3000);
	Pump pumpHeat = Pump(24, LOW, 3000, 3000);

	//void StopCompressor();
	//void StartCompressor();

	
};

