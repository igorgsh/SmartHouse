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
	String BoardName = "";
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	IPAddress ip= IPAddress(192, 168, 0, 90);

	bool IsConfigReady=false;
	bool IsActionsReady = false;
	static const int MqttPort = 1883;
	static const int TRY_NUMBER_FOR_CONNECT = 5;


	bool isConfigRequested = false;
	bool isActionRequested = false;


	Unit** units = NULL;
	Action** actions = NULL;

	void Init();
	void UpdateConfig(String jsonConfig);
	void UpdateActions(String jsonConfig);
	void BuildActions();
	Unit* FindUnit(uint16_t id);
	//Unit* FindUnitByTypeAndPin(UnitType type, byte pin);
	void UpdateButton(String unit, String value) { UpdateUnit(UnitType::BUTTON, unit, value); };
	void UpdateRelay(String unit, String value) { UpdateUnit(UnitType::RELAY, unit, value); };
	void UpdateOneWireBus(String button, String value) { UpdateUnit(UnitType::ONE_WIRE_BUS, button, value); };
	void UpdateOneWireThermo(String button, String value) { UpdateUnit(UnitType::ONE_WIRE_THERMO, button, value); };
	void UpdateUnit(UnitType type, String name, String value);
	void UpdatePowerMeter(String unit, String value) { UpdateUnit(UnitType::POWER_METER, unit, value); };
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

//	StaticJsonBuffer<200> jsonBuffer;

};