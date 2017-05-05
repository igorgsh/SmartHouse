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

#define NUMBER_OF_TEMP 2
#define NUMBER_OF_CONTACTOR 1
#define NUMBER_OF_PUMPS 3
#define NUMBER_OF_OUTPUTDEVICES (1 + NUMBER_OF_PUMPS)

extern int __bss_end;
extern int *__brkval;



class Configuration
{
public:
	Configuration();
	~Configuration();
	//EthernetServer* server;
	/*
	Pump pumps[NUMBER_OF_PUMPS] = {
										Pump(22,LOW),
										Pump(23, LOW),
										Pump(24, LOW),
	};
	Compressor compressor = Compressor(21, LOW);
	*/
	OutputManager outputManager = OutputManager(&compressor,pumps);

//	ActionScenario StartPump = DeviceItem(&PumpItem[0], ScenarioCommand::START);
	ActionScenario StartCompressor = ActionScenario({
		DeviceItem(outputManager.GetDevice(compressor), ScenarioCommand::START),
	});

	OutputDevice outputDevices[NUMBER_OF_OUTPUTDEVICES] = { Compressor(21, LOW), Pump(22,LOW) };

	TempSensor tempSensors[NUMBER_OF_TEMP] = {	TempSensor("D1",30,20.0,24.0,21.0,23.0, NULL, 5),
												TempSensor("D2",31,21.0,28.0,22.0,25.0, &relays[0], 6) };
	Contactor contacts[NUMBER_OF_CONTACTOR] = { Contactor("C1",23,0, &relays[1], 5) };
	CentralThermo cThermo = CentralThermo(&tempSensors[0], 35);
	

	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	void begin();
	bool loop(unsigned long counter);
	static int memoryFree();
	unsigned long counter100 = 0;
	unsigned long counter500 = 0;
	unsigned long counter1000 = 0;
private:
	ArduinoServer* web;
	// IP address in case DHCP fails
	//IPAddress* ip;
	//IPAddress ip = new IPAddress()
};

