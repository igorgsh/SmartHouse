#pragma once

#include <Arduino.h>
#include "definitions.h"
#include "Loger.h"
#include "Action.h"
#include <ArduinoJson.h>



class Unit
{
public:
	uint16_t Id;
	byte Type;
	byte status;
	uint16_t parentId=0;
	byte Pin;
	//byte parentPin = 0;

	
	//bool isSubscribed = false;

	virtual bool Compare(const Unit* u) =0;
	virtual void FillFrom(const Unit* u);
	virtual void UnitLoop(unsigned long timePeriod, bool isParent = false, bool val = false) = 0;
	virtual void InitUnit(bool isParent=false) = 0;
	virtual void FinalInitUnit(bool isParent = false) = 0;
	virtual byte UnitStoredSize() = 0;
	virtual void ReadFromEEPROM(uint16_t addr) = 0;
	virtual void WriteToEEPROM(uint16_t addr) = 0;
	// All units are initialized. Link Bus devices
	virtual void ProcessUnit(ActionType action) = 0;
	virtual ~Unit() {};
	virtual void const print(const char* header, DebugLevel level);
	virtual void ConfigField(const JsonObject& jsonList) = 0;
	static void PublishTypedUnit(UnitType ut, uint16_t id, byte status);

protected:
	void PublishUnit(const char* uPrefix);
};


class UnitProto : public Unit {
public:
	void InitUnit(bool isParent) {};
	void ProcessUnit(ActionType action) {};
	void UnitLoop(unsigned long timePeriod, bool isParent, bool val) {};
	void FinalInitUnit(bool isParent) {};
	byte UnitStoredSize() {return 0;};
	void ReadFromEEPROM(uint16_t addr) {};
	void WriteToEEPROM(uint16_t addr) {};
	void ConfigField(const JsonObject& jsonList) {};
	bool Compare(const Unit* u) { return false; };

};
