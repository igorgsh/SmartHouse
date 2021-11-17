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
	virtual void UnitLoop() = 0;
	virtual void InitUnit() = 0;
	virtual void FinalInitUnit() = 0;
	virtual void ParentUnitLoop(bool v) = 0;
	virtual void ParentInitUnit() = 0;
	virtual void ParentFinalInitUnit() = 0;
	virtual byte UnitStoredSize() = 0;
	virtual void ReadFromEEPROM(uint16_t addr) = 0;
	virtual void WriteToEEPROM(uint16_t addr) = 0;
	// All units are initialized. Link Bus devices
	virtual void ProcessUnit(ActionType action) = 0;
	virtual ~Unit() {};
	virtual void const print(const char* header, DebugLevel level);
	virtual void ConfigField(const JsonObject& jsonList) = 0;
};

class UnitProto : public Unit {
public:
	void InitUnit() {};
	void ProcessUnit(ActionType action) {};
	void UnitLoop() {};
	void FinalInitUnit() {};
	byte UnitStoredSize() {return 0;};
	void ReadFromEEPROM(uint16_t addr) {};
	void WriteToEEPROM(uint16_t addr) {};
	void ConfigField(const JsonObject& jsonList) {};
	bool Compare(const Unit* u) { return false; };
	void ParentUnitLoop(bool v) {};
	void ParentInitUnit() {};
	void ParentFinalInitUnit() {};

};
