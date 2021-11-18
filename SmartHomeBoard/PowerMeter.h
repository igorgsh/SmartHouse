#pragma once
#include "Unit.h"
#include <PZEM004T.h>


typedef enum {
	PM_VOLTAGE,
	PM_CURRENT,
	PM_POWER,
	PM_ENERGY
} PowerMeterValues;

class PowerMeter :
	public Unit
{
public:
	

	PowerMeter();
	~PowerMeter();
	
	double voltage();
	double current();
	double power();
	double energy();

	void InitUnit();
	void UnitLoop();
	void FinalInitUnit();

	void ParentInitUnit() {};
	void ParentUnitLoop(bool v) {};
	void ParentFinalInitUnit() {};


	void PublishAll();
	void ProcessUnit(ActionType action);

	byte serialNumber = 0;
	byte serialRX = 0;
	byte serialTX = 0;
	int factor = 1;

	void static MqttTopic(uint16_t unitId, char *topic, PowerMeterValues val);

	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 7; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);


private:
	//HardwareSerial *port=NULL;
	PZEM004T* pzem=NULL;
	IPAddress ip = IPAddress(10, 10, 10, 10);
	//Stream *port = NULL;

};

