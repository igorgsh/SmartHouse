// configuration.h
#pragma once
#include <Ethernet.h>
#include "Unit.h"
#include "Action.h"
#include <ArduinoJson.h>

#define JSON_SIZE	200
class Configuration {
public:
	bool IsEthernetConnection=false;

	byte BoardId = 0;
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	// IP address in case DHCP fails
	IPAddress ip= IPAddress(192, 168, 0, 101);
	bool IsConfigReady=false;
	bool IsActionsReady = false;
	static const int MqttPort = 1883;
	bool IsServerConfig = false;
	bool IsServerActions = false;

	Unit** units = NULL;
	Action** actions = NULL;

	void Init();
	void UpdateConfig(const char *jsonConfig);
	void UpdateActions(const char *jsonConfig);
	void BuildConfig();
	void BuildActions();
	Unit* FindUnit(byte id);
	Unit* FindUnitByTypeAndPin(UnitType type, byte pin);
	//	void UpdateStorage(const Unit* u);
	void StoreUnits();
	void ReadUnit(int i, Unit* u);
	void WriteUnit(int i, const Unit* u);

	void StoreActions();
	void ReadAction(int i, Action* u);
	void WriteAction(int i, const Action* u);
	void UpdateButton(String button, String value) { UpdateUnit(UnitType::BUTTON, button, value); };
	void UpdateRelay(String button, String value) { UpdateUnit(UnitType::BUTTON, button, value); };
	void UpdateOneWireBus(String button, String value) { UpdateUnit(UnitType::ONE_WIRE_BUS, button, value); };
	void UpdateOneWireThermo(String button, String value) { UpdateUnit(UnitType::ONE_WIRE_THERMO, button, value); };
	void UpdateUnit(UnitType type, String name, String value);
	void UnitsLoop();
	void ProcessAction(byte id, byte event, unsigned long value);

	static const char* MqttServer() {
		return "192.168.0.32"; //IP-адрес сервера ioBroker
	}


	int numberUnits = 0;
	int numberActions = 0;
	int numberBusUnits = 0;

private:
	int configCounter;
	int actionCounter;


	// EEPROM structure
	//	Byte | Object
	//	-------------
	//	0:	BoardId
	static const int addrBoardId = 0;
	//	1: Number Of Units 
	static const int addrNumberUnits = 1;
	//	2: Number of Actions
	static const int addrNumberActions = 2;
	//	3: Number of bus units
	static const int addrNumberBusUnits = 3;
	//	4: Reserved
	//	5: Start of Units
	static const int addrUnits = 5;
	static const int sizeOfUnit = 4;
	// Unit structure
	//	Byte | Object
	//	-------------
	//	0: id
	//	1: type
	//	2: pin
	//	3: lhOn
	int GetOneWireStartAddr(int i);
	static const int sizeOfBusUnit = 8;
	// Action structure
	//	Byte | Object
	//	-------------
	//	0-7: address
	int GetActionsStartAddr(int i);
	static const int sizeOfAction = 6;
	// Action structure
	//	Byte | Object
	//	-------------
	//	0: id
	//	1: originId
	//	2: originType
	//	3: event
	//	4: targetId
	//	5: targetAction


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

//	StaticJsonBuffer<200> jsonBuffer;

};