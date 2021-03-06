#pragma once
#include "Unit.h"
#include <OneWire.h>
#include "DallasTemperature.h"
#include "SigmaEEPROM.h"

#define BUS_RESOLUTION 9
#define BUS_INTERVAL  2000

class OneWireBus :
	public Unit
{
public:
	byte Pin;

	void InitUnit();
	void ProcessUnit(ActionType action);
	void UnitLoop();
	void SetDefault();
	void FinalInitUnit();
	float GetTemperature(const DeviceAddress address);
	bool CheckAddress(const DeviceAddress address);
	void SetResolution(const DeviceAddress address);
	void RequestTemperature();
	bool Compare(const Unit* u);
	byte UnitStoredSize() { return 3; }
	void ReadFromEEPROM(uint16_t addr);
	void WriteToEEPROM(uint16_t addr);
	static bool CompareDeviceAddress(const DeviceAddress a0, const DeviceAddress a1);
	void ConfigField(const JsonObject& jsonList);
	static void ConvertStringToAddress(DeviceAddress address, const String &addrStr);
	static void ConvertAddressToString(const DeviceAddress address, String &strAddress);
	void const print(const char* header, DebugLevel level);

private:
	OneWire* oneWire;
	int numberThermo = 0;
	DallasTemperature *sensors;
	unsigned long prevCycle = 0;

};

