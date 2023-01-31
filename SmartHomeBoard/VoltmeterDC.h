#pragma once
#include "Unit.h"

#include <INA3221.h>

class VoltmeterDC :
    public Unit
{
public:
	VoltmeterDC();
	~VoltmeterDC();


	void InitUnit(bool isParent);
	void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
	void FinalInitUnit(bool isParent) {};


	void ProcessUnit(ActionType action);


	//void static MqttTopic(uint16_t unitId, char* topic, PowerMeterValues val);

	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 11; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);

private:
	byte version = 3;
	ina3221_addr_t address = INA3221_ADDR40_GND;
	int resistance[3] = { 100, 100, 100 };
	INA3221* voltmeter3 = NULL;
	double voltage[3];
	double current[3];
	double currentComp[3];
	
	void callSensor();

	void publishVoltmeter();
};

