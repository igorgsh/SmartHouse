// configuration.h
#pragma once
#include <Ethernet.h>
#include "Unit.h"
#include "Action.h"
#include "DallasTemperature.h"
#include <ArduinoJson.h>

#define JSON_SIZE	200
class Configuration {
public:
	bool IsEthernetConnection=true;

	byte BoardId = 0;
	char BoardName[10];
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	IPAddress ip= IPAddress(192, 168, 0, 90);
	char strIP[20];

	bool IsConfigReady=false;
	bool IsActionsReady = false;
	static const int MqttPort = 1883;
	static const int TRY_NUMBER_FOR_CONNECT = 5;


	bool isConfigRequested = false;
	bool isActionRequested = false;


	Unit** units = NULL;
	Action** actions = NULL;

	void Init();
	void UpdateConfig(const char* jsonConfig);
	void UpdateActions(const char* jsonConfig);
	void BuildActions();
	Unit* FindUnit(uint16_t id);
	void UpdateButton(uint16_t unitId, uint16_t value) { UpdateUnit(UnitType::BUTTON, unitId, value); };
	void UpdateRelay(uint16_t unitId, uint16_t value) { UpdateUnit(UnitType::RELAY, unitId, value); };
	void UpdateOneWireBus(uint16_t unitId, uint16_t value) { UpdateUnit(UnitType::ONE_WIRE_BUS, unitId, value); };
	void UpdateOneWireThermo(uint16_t unitId, uint16_t value) { UpdateUnit(UnitType::ONE_WIRE_THERMO, unitId, value); };
	void UpdateUnit(UnitType type, uint16_t id, uint16_t value);
	void UpdatePowerMeter(uint16_t unitId, uint16_t value) { UpdateUnit(UnitType::POWER_METER, unitId, value); };
	void UpdateContactor(uint16_t unitId, uint16_t value) { UpdateUnit(UnitType::CONTACTOR, unitId, value); };
	void ProcessAction(uint16_t id, byte event);
	Unit* CreateTypedUnit(byte type);

	void MainLoop();

	static const char* MqttServer() {
		return "192.168.0.99"; //IP-адрес сервера ioBroker
	}


	int numberUnits = 0;
	int numberActions = 0;

	//Timer 
	//unsigned long counter01 = 0;	//100ms
	//unsigned long counter05 = 0;	//500ms
	//unsigned long counter1 = 0;		//1 sec
	unsigned long counter30 = 0;	//30 sec
	unsigned long counter60 = 0;	//1min
	//unsigned long counter300 = 0;	//5min

	//void loop01();
	//void loop05();
	//void loop1();
	void loop30();
	void loop60();
	//void loop300();
	Unit** CreateUnits(byte nUnits);
	Action** CreateActions(byte nActions);

private:
	int configCounter;
	int actionCounter;



	static const byte addrBoardId = 0;

	void ReadBoardId();
	void InitializeUnits();
	void FinalizeInitUnits();
	void InitializeActions();
	void BuildConfig();
	void InitializeServer();
	void UnitsLoop();


};