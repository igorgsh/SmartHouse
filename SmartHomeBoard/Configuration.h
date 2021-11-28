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
	Mqtt* MqttClient; 
	bool IsEthernetConnection=true;
	EthernetClient* EthClient;
	Loger* Log;

	//Mqtt MqttClient;
	byte BoardId = 0;
	char BoardName[10];
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	IPAddress ip= IPAddress(192, 168, 0, 90);
	IPAddress mqttIp = IPAddress(192, 168, 0, 99);
	//char strIP[16];

	bool IsConfigReady=false;
	bool IsActionsReady = false;
	static const int MqttPort = 1883;
	static const int TRY_NUMBER_FOR_CONNECT = 5;


	bool isConfigRequested = false;
	bool isActionRequested = false;

	unsigned long Delay_OFF_6 = 0;
	unsigned long Delay_ON_7 = 0;


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
	void Pass2Parent(uint16_t parentId, byte parentPin, bool status);

	Unit* CreateTypedUnit(byte type);

	//void MainLoop();
	/*
	static const char* MqttServer() {
		return "192.168.0.99"; //IP-адрес сервера ioBroker
	}
	*/
	//static const char* MqttServer() { return "192.168.0.99"; };

	int numberUnits = 0;
	int numberActions = 0;

	//Timer 
	unsigned long counter10 = 0;	//10ms
	unsigned long counter1 = 0;		//1 sec
	unsigned long counter30 = 0;	//30 sec
	unsigned long counter60 = 0;	//1min
	//unsigned long counter300 = 0;	//5min

	//void loop01();
	void Loop(unsigned long timePeriod);
	Unit** CreateUnits(byte nUnits);
	Action** CreateActions(byte nActions);
	void UpdateCommonParams(const char* topic, const char* payload);

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
	void UnitsLoop(unsigned long timePeriod);


};