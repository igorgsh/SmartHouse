#pragma once
#include "Unit.h"
#include <SoftwareSerial.h>
#include <PZEM004T.h>
#include <PZEM004Tv30.h>

typedef enum {
	PM_VOLTAGE,
	PM_CURRENT,
	PM_POWER,
	PM_ENERGY,
	PM_FREQUENCY,
	PM_POWERFACTOR
} PowerMeterValues;

class PowerMeter :
	public Unit
{
public:
	

	PowerMeter();
	~PowerMeter();
	
	double Voltage();
	double Current();
	double Power();
	double Energy();
	double Frequency();
	double PowerFactor();

	void InitUnit(bool isParent);
	void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
	void FinalInitUnit(bool isParent);


	//void PublishAll();
	void PublishPowerMeter(PowerMeterValues step, double v);

	void ProcessUnit(ActionType action);

	byte serialNumber = 0;
	byte serialRX = 0;
	byte serialTX = 0;
	//byte factor = 1;
	byte version = 2;

	void static MqttTopic(uint16_t unitId, char *topic, PowerMeterValues val);

	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 7; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	void ConfigField(const JsonObject& jsonList);
	void const print(const char* header, DebugLevel level);


private:
	HardwareSerial *hardPort=NULL;
	SoftwareSerial *softPort = NULL;
	PZEM004T* pzem = NULL;
	//PZEM004Tv30* pzem3 = NULL;
	IPAddress ip = IPAddress(10, 10, 10, 10);
	PowerMeterValues step = PM_VOLTAGE;

};

