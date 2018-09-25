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
	// IP address in case DHCP fails
	IPAddress ip= IPAddress(192, 168, 0, 101);
	bool IsConfigReady=false;
	bool IsActionsReady = false;
	static const int MqttPort = 1883;
	static const int TRY_NUMBER_FOR_CONNECT = 5;
	bool IsServerConfig = false;
	bool IsServerActions = false;


	bool isConfigRequested = false;
	bool isActionRequested = false;


	Unit** units = NULL;
	Action** actions = NULL;

	void Init();
	void UpdateConfig(String jsonConfig);
	void UpdateActions(String jsonConfig);
	void BuildActions();
	Unit* FindUnit(uint16_t id);
	Unit* FindUnitByTypeAndPin(UnitType type, byte pin);
	//	void UpdateStorage(const Unit* u);
	void StoreUnits();
	void ReadUnit(int i, Unit* u);
	void WriteUnit(int i, const Unit* u);

	void StoreActions();
	void ReadAction(int i, Action* u);
	void WriteAction(int i, const Action* u);
	void UpdateButton(String unit, String value) { UpdateUnit(UnitType::BUTTON, unit, value); };
	void UpdateRelay(String unit, String value) { UpdateUnit(UnitType::RELAY, unit, value); };
	void UpdateOneWireBus(String button, String value) { UpdateUnit(UnitType::ONE_WIRE_BUS, button, value); };
	void UpdateOneWireThermo(String button, String value) { UpdateUnit(UnitType::ONE_WIRE_THERMO, button, value); };
	void UpdateUnit(UnitType type, String name, String value);
	void ProcessAction(uint16_t id, byte event);
	//bool CheckConfigReady();
	//bool CheckActionReady();

	void MainLoop();

	static const char* MqttServer() {
		return "192.168.0.33"; //IP-адрес сервера ioBroker
	}


	int numberUnits = 0;
	int numberActions = 0;
	int numberBusUnits = 0;
	void ConvertStringToAddress(DeviceAddress address, const String addrStr);
	String ConvertAddressToString(const DeviceAddress address);

	//Timer 
	unsigned long counter01 = 0;	//100ms
	unsigned long counter05 = 0;	//500ms
	unsigned long counter1 = 0;		//1 sec
	unsigned long counter60 = 0;	//1min
	unsigned long counter300 = 0;	//5min

	void loop01();
	void loop05();
	void loop1();
	void loop60();
	void loop300();

private:
	int configCounter;
	int actionCounter;


	uint16_t Read16(uint16_t addr);
	void Write16(uint16_t addr, uint16_t val);


	// EEPROM structure
	//	Byte | Object
	//	-------------
	//	0:	BoardId
	static const byte addrBoardId = 0;
	//	1: Number Of Units 
	static const byte addrNumberUnits = 1;
	//	2: Number of Actions
	static const byte addrNumberActions = 2;
	//	3: Number of bus units
	static const byte addrNumberBusUnits = 3;
	//	4: Reserved
	//	5: Start of Units
	static const byte addrUnits = 5;
	int GetUnitsAddr(int i);
	static const byte sizeOfUnit = 5;
	// Unit structure
	//	Byte | Object
	//	-------------
	//	0-1: id
	//	2: type
	//	3: pin
	//	4: lhOn
	int GetOneWireAddr(int i);
	static const int sizeOfBusUnit = 8;
	// Action structure
	//	Byte | Object
	//	-------------
	//	0-7: address
	int GetActionsAddr(int i);
	static const int sizeOfAction = 10;
	// Action structure
	//	Byte | Object
	//	-------------
	//	0-1: id
	//	2-3: originId
	//	4: originType
	//	5: event
	//	6-7: targetId
	//	8: targetAction
	//	9: targetAction


	void WriteNumberUnits();
	void ReadNumberUnits();
	void WriteNumberActions();
	void ReadNumberActions();
	void WriteNumberBusUnits();
	void ReadNumberBusUnits();
	void CreateUnits();
	void ReadBoardId();
	void InitializeUnits();
	void FinalizeInitUnits();
	void CreateActions();
	void InitializeActions();
	Unit* CreateTypedUnit(byte type);
	void BuildConfig();
	void InitializeServer();
	void UnitsLoop();

//	StaticJsonBuffer<200> jsonBuffer;

};