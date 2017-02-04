// configuration.h
#pragma once
#include "types.h"
#include <Ethernet.h>
#include "Unit.h"
#include "Action.h"

class Configuration {
public:
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
	Unit* FindUnit(byte id, byte type);
//	void UpdateStorage(const Unit* u);
	void StoreUnits();
	void ReadUnit(int i, Unit* u);
	void WriteUnit(int i, const Unit* u);

	void StoreActions();
	void ReadAction(int i, Action* u);
	void WriteAction(int i, const Action* u);
	void UpdateButton(String button, String value) { UpdateUnit(UnitType::BUTTON, button, value); };
	void UpdateRelay(String button, String value) { UpdateUnit(UnitType::BUTTON, button, value); };
	void UpdateUnit(UnitType type, String name, String value);

	static const char* MqttServer() {
		return "192.168.0.32"; //IP-адрес сервера ioBroker
	}
	int numberUnits = 0;

private:
	int configCounter;
	int numberActions = 0;
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
	//	3: Reserved
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
	int GetActionsStartAddr();
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
	void CreateUnits();
	void ReadBoardId();
	void InitializeUnits();
	void CreateActions();
	void InitializeActions();

};