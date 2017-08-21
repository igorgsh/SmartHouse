#pragma once
#include "Sensor.h"
#include "TempSensor.h"
#include "Contactor.h"
#include "Relay.h"
#include "CentralThermo.h"
#include "ArduinoServer.h"
#include "ScenarioItem.h"
#include "Pump.h"
#include "Compressor.h"
#include "DeviceItem.h"
#include "DeviceManager.h"

#define NUMBER_OF_TEMP 2
#define NUMBER_OF_CONTACTOR 1
#define NUMBER_OF_PUMPS 4
#define NUMBER_OF_OUTPUTDEVICES (1 + NUMBER_OF_PUMPS)

#define NUMBER_ITEM_COMPRESSOR_START 5
#define NUMBER_ITEM_COMPRESSOR_STOP 6


extern int __bss_end;
extern int *__brkval;



class Configuration
{
public:
	Configuration();
	~Configuration();

	DeviceManager DevManager = DeviceManager();

	ActionScenario StartCompressor = ActionScenario(NUMBER_ITEM_COMPRESSOR_START, compressorStart);
	ActionScenario StopCompressor = ActionScenario(NUMBER_ITEM_COMPRESSOR_STOP, compressorStop);


	OutputDevice* outputDevices[NUMBER_OF_OUTPUTDEVICES] = { &compressor, &pumpGeo, &pumpContour1, &pumpContour2, &pumpHeat };

	TempSensor tempSensors[NUMBER_OF_TEMP] = { TempSensor("D1",30,20.0,24.0,21.0,23.0, NULL, 5),
												TempSensor("D2",31,21.0,28.0,22.0,25.0, &relays[0], 6) };
	Contactor contacts[NUMBER_OF_CONTACTOR] = { Contactor("C1",23,0, &relays[1], 5) };
	CentralThermo cThermo = CentralThermo(&tempSensors[0], 35);


	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	void begin();
	bool loop1(unsigned long counter);
	static int memoryFree();
	unsigned long counter1 = 0;
	unsigned long counter5 = 0;
	unsigned long counter10 = 0;
private:
	ArduinoServer* web;

	// IP address in case DHCP fails
	//IPAddress* ip;
	//IPAddress ip = new IPAddress()
	/*
	ScenarioItem* compressorStart[NUMBER_ITEM_COMPRESSOR_START]
	{ 
		new DeviceItem(&pumpGeo, ScenarioCommand::COMMAND_START),
		new DeviceItem(&pumpContour1, ScenarioCommand::COMMAND_START),
		new DeviceItem(&pumpContour1, ScenarioCommand::COMMAND_START),
		new DeviceItem(&pumpHeat, ScenarioCommand::COMMAND_START),
		new DeviceItem(&compressor, ScenarioCommand::COMMAND_START)
	};

	ScenarioItem* compressorStop[NUMBER_ITEM_COMPRESSOR_STOP]
	{
		new DeviceItem(&compressor, ScenarioCommand::COMMAND_STOP),
		new SleepItem(60 * 1000),
		new DeviceItem(&pumpGeo, ScenarioCommand::COMMAND_STOP),
		new DeviceItem(&pumpContour1, ScenarioCommand::COMMAND_STOP),
		new DeviceItem(&pumpContour1, ScenarioCommand::COMMAND_STOP),
		new DeviceItem(&pumpHeat, ScenarioCommand::COMMAND_STOP),
	};
	*/
};

