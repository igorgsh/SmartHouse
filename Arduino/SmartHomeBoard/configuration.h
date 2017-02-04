// configuration.h
#pragma once
#include "types.h"
#include <Ethernet.h>
#include "Unit.h"

/*
void UpdateConfiguration(const ButtonUnit* unit);
void UpdateConfiguration(const LightUnit* unit);
void UpdateConfiguration(const RelayUnit* unit);
void UpdateConfiguration(const ActionUnit* unit);

void DeleteConfiguration(const ButtonUnit* unit);
void DeleteConfiguration(const LightUnit* unit);
void DeleteConfiguration(const RelayUnit* unit);
void DeleteConfiguration(const ActionUnit* unit);
*/
class Configuration {
public:
	int numberUnits = 0;
	byte BoardId = 0;
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x00 };
	// IP address in case DHCP fails
	IPAddress ip= IPAddress(192, 168, 0, 101);
	//const PROGMEM byte ServerPort = 80;
	bool IsConfigReady=true;
	static const int MqttPort = 1883;
//	static const IPAddress MqttServer = IPAddress(192, 168, 0, 32);

	Unit **units = NULL;

	void Init();
	void updateConfig(const char *jsonConfig);
	void BuildConfig();
	Unit* FindUnit(byte id, byte type);
//	void UpdateStorage(const Unit* u);
	void UpdateStorage();
	void ReadUnit(int i, Unit* u);
	void WriteUnit(int i, const Unit* u);

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

	
	static const char* MqttServer() {
		return "192.168.0.32"; //IP-адрес сервера ioBroker
	}
	
private:
	int configCounter;
	void WriteNumberUnits();
	void ReadNumberUnits();
	void CreateUnits();
	void ReadBoardId();

};