// configuration.h
#pragma once
#include <Ethernet.h>
#include "Unit.h"
#include "Action.h"
#include "DallasTemperature.h"
#include <ArduinoJson.h>
#include "Mqtt.h"


#define JSON_SIZE	200
class Configuration {
public:
<<<<<<< HEAD
	bool IsEthernetConnection = true;
=======
	Mqtt* MqttClient; 
	bool IsEthernetConnection=true;
	EthernetClient* EthClient;
	Loger* Log;
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516

	//Mqtt MqttClient;
	byte BoardId = 0;
<<<<<<< HEAD
	String BoardName;
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	IPAddress ip = IPAddress(192, 168, 0, 90);
=======
	char BoardName[10];
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	IPAddress ip= IPAddress(192, 168, 0, 90);
	char strIP[20];
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516

	bool IsConfigReady = false;
	bool IsActionsReady = false;
	static const int MqttPort = 1883;
	static const int TRY_NUMBER_FOR_CONNECT = 5;
	 

	bool isConfigRequested = false;
	bool isActionRequested = false;


	Unit** units = NULL;
	Action** actions = NULL;

	Configuration();
	void Init();
<<<<<<< HEAD
	void UpdateConfig(const String& jsonConfig);
	void UpdateActions(const String& jsonConfig);
	void BuildActions();
	Unit* FindUnit(uint16_t id);
	void UpdateButton(int id, const String& value) { UpdateUnit(UnitType::BUTTON, id, value); };
	void UpdateRelay(int id, const String& value) { UpdateUnit(UnitType::RELAY, id, value); };
	void UpdateOneWireBus(int id, const String& value) { UpdateUnit(UnitType::ONE_WIRE_BUS, id, value); };
	void UpdateOneWireThermo(int id, const String& value) { UpdateUnit(UnitType::ONE_WIRE_THERMO, id, value); };
	void UpdateUnit(UnitType type, int id, const String& value);
	void UpdatePowerMeter(int id, const String& value) { UpdateUnit(UnitType::POWER_METER, id, value); };
	void UpdateContactor(int id, const String& value) { UpdateUnit(UnitType::CONTACTOR, id, value); };
	void UpdateShiftRegisterIn(int id, const String& value) { UpdateUnit(UnitType::SHIFT_IN, id, value); };
	void UpdateShiftRegisterOut(int id, const String& value) { UpdateUnit(UnitType::SHIFT_OUT, id, value); };
=======
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
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	void ProcessAction(uint16_t id, byte event);
	Unit* CreateTypedUnit(byte type);

	void MainLoop();
	/*
	static const char* MqttServer() {
		return "192.168.0.99"; //IP-адрес сервера ioBroker
	}
	*/
	static const char* MqttServer() { return "192.168.0.99"; };

	int numberUnits = 0;
	int numberActions = 0;

	//Timer 
	//unsigned long counter01 = 0;	//100ms
	unsigned long counter10 = 0;	//10ms
	unsigned long counter1 = 0;		//1 sec
	unsigned long counter30 = 0;	//30 sec
	unsigned long counter60 = 0;	//1min
	//unsigned long counter300 = 0;	//5min

	//void loop01();
	void loop10();
	void loop1();
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

<<<<<<< HEAD
=======

>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
};