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

	/*
	virtual void FillFrom(Unit* u);
	virtual void const print(const char* header, DebugLevel level);
	*/
	int serialNumber = -1;
	int serialRX = -1;
	int serialTX = -1;
	double factor = 1.0;

	void static MqttTopic(uint16_t unitId, char *topic, PowerMeterValues val);

private:
	//HardwareSerial *port=NULL;
	PZEM004T* pzem=NULL;
	IPAddress ip = IPAddress(10, 10, 10, 10);
	//Stream *port = NULL;

};

