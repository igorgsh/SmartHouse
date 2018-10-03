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
	void InitUnit();
	
	double voltage();
	double current();
	double power();
	double energy();

	void UnitLoop();
	void PublishAll();
	void SetDefault();
	void FinalInitUnit();
	void ProcessUnit(ActionType action);
	//bool compare(Unit* u);

	/*
	virtual void FillFrom(Unit* u);
	virtual void const print(const char* header, DebugLevel level);
	*/
	byte serialNumber = 0;
	byte serialRX = 0;
	byte serialTX = 0;
	byte factor = 1;

	void static MqttTopic(uint16_t unitId, char *topic, PowerMeterValues val);

private:
	//HardwareSerial *port=NULL;
	PZEM004T* pzem=NULL;
	IPAddress ip = IPAddress(10, 10, 10, 10);
	//Stream *port = NULL;

};

