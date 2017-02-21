#pragma once
#include "Sensor.h"
#include "TempSensor.h"
#include "Contactor.h"
#include "Relay.h"
#include "CentralThermo.h"
#include "ArduinoServer.h"

#define NUMBER_OF_TEMP 2
#define NUMBER_OF_CONTACTOR 1

class Configuration
{
public:
	Configuration();
	~Configuration();
	//EthernetServer* server;
	Relay relays[2] = { Relay(22,LOW),
					    Relay(24,LOW)	};
	TempSensor tempSensors[NUMBER_OF_TEMP] = {	TempSensor("D1",30,20.0,24.0,21.0,23.0, NULL, 50),
												TempSensor("D2",31,20.0,24.0,21.0,23.0, &relays[0], 50) };
	Contactor contacts[NUMBER_OF_CONTACTOR] = { Contactor("C1",23,LOW, &relays[1], 5) };
	CentralThermo cThermo = CentralThermo(&tempSensors[0], 35);

	
	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	void begin();
	bool loop(unsigned long counter);
private:
	ArduinoServer* web;
	// IP address in case DHCP fails
	//IPAddress* ip;
	//IPAddress ip = new IPAddress()
};

