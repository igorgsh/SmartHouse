// 
// 
// 

#include "configuration.h"
#include "mqtt.h"
#include <ArduinoJson.h>
#include "button.h"
#include "relay.h"
#include "EEPROM.h"
#include "Unit.h"
#include "Board.h"
#include "Loger.h"

extern Mqtt MqttClient;

void Configuration::CreateUnits() {
	if (units != NULL) {
		free(units);
	}
	units = (Unit**)malloc(numberUnits * sizeof(Unit*));
	//IsConfigReady = false;
}

Unit* Configuration::FindUnit(byte id, byte type) {
	
	if (units != NULL && IsConfigReady ) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Id == id && units[i]->Type == type) {
				return units[i];
			}
		}
	}
	return NULL;
}

void Configuration::Init() {
	ReadBoardId();
}

Unit* CreateTypedUnit(byte type) {
	Unit *u = NULL;
	if (type == UnitType::BUTTON) {
		Debug("Button");
		u = new Button();
		if (u == NULL) {
			Loger::Error("Can't create Button Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {
		Debug("Relay");
		u = new Relay();
		if (u == NULL) {
			Loger::Error("Can't create Button Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	return u;
}

void Configuration::updateConfig(const char *jsonConfig) {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
	Debug("Update Config");
	Debug2("IsConfigReady:", IsConfigReady);
	root.prettyPrintTo(Serial);
	if (root.containsKey("length")) {
		Debug2("Length:", (int)root["length"]);

		int n = (int)root["length"];
		if (n != numberUnits) {
			WriteNumberUnits();
			numberUnits = n;
		}
		configCounter = 0;
		CreateUnits();
	}
	else if (!IsConfigReady && root.containsKey("type") && root.containsKey("id")) {
		Debug2("N:", (int)root["N"]);
		Debug2("type0:", ((const char*)root["type"])[0]);
		//Unit *u = units[configCounter];
		units[configCounter] = CreateTypedUnit(((const char*)root["type"])[0]);
		/*
		if (((const char*)root["type"])[0] == UnitType::BUTTON) {
			Debug("Button");
			units[configCounter] = new Button();
			units[configCounter]->Type = UnitType::BUTTON;
		}
		else if (((const char*)root["type"])[0] == UnitType::RELAY) {
			Debug("Relay");
			units[configCounter] = new Relay();
			units[configCounter]->Type = UnitType::RELAY;
		}
		*/
		if (units[configCounter] != NULL) {
			if (root.containsKey("id")) {
				units[configCounter]->Id = root["id"];
			}
			if (root.containsKey("Pin")) {
				units[configCounter]->Pin = root["Pin"];
			}
			if (root.containsKey("lhOn")) {
				units[configCounter]->lhOn = root["lhOn"];
			}
			if (root.containsKey("status")) {
				units[configCounter]->status = root["status"];
			}
		}
		Debug_("Id:");
		Debug_(units[configCounter]->Id);
		Debug_(";type:");
		Debug_(units[configCounter]->Type);
		Debug_(";Pin:");
		Debug_(units[configCounter]->Pin);
		Debug_(";lhOn:");
		Debug_(units[configCounter]->lhOn);
		Debug("#");

		configCounter++;
		if (configCounter == numberUnits) {
			Debug("Finish update configuration");
			IsConfigReady = true;
			UpdateStorage();
		}
	}

}
/*
void Configuration::UpdateStorage(const Unit* u) {

}
*/

void Configuration::BuildConfig() {
	IsConfigReady = false;
	ReadNumberUnits();
	MqttClient.GetConfiguration();
	unsigned long startServerRead = millis();
	while (!IsConfigReady && startServerRead+10000 > millis()) { // we have a 10 seconds to get configuration
		MqttClient.MqttLoop();
	}
	if (!IsConfigReady) { // We can't get config from server. Read from EEPROM
		Debug("Get From EEPROM");
		CreateUnits();
		for (int i = 0; i < numberUnits; i++) {
			Unit u0;
			ReadUnit(i, &u0);
			units[i] = CreateTypedUnit(u0.Type);
			units[i]->SetDefault();

			Debug_("Id:");
			Debug_(units[i]->Id);
			Debug_(";type:");
			Debug_(units[i]->Type);
			Debug_(";Pin:");
			Debug_(units[i]->Pin);
			Debug_(";lhOn:");
			Debug_(units[i]->lhOn);
			Debug("#");

		}
		Debug("Done!");
	}
}

void Configuration::ReadBoardId() {

	BoardId = EEPROM.read(addrBoardId);
	mac[3] = BoardId;
}

void Configuration::ReadUnit(int i, Unit* u) {
	/*
		Byte | Object
		-------------
		0: id
		1: type
		2: pin
		3: lhOn
	*/
	int addr = Configuration::addrUnits + i * Configuration::sizeOfUnit;

	u->Id = EEPROM.read(addr);
	u->Type = EEPROM.read(addr + 1);
	u->Pin = EEPROM.read(addr + 2);
	u->lhOn = EEPROM.read(addr + 3);

}

void Configuration::WriteUnit(int i, const Unit* u) {
	/*
	Byte | Object
	-------------
	0: id
	1: type
	2: pin
	3: lhOn
	*/
	int addr = Configuration::addrUnits + i * sizeOfUnit;

	EEPROM.write(addr, u->Id);
	EEPROM.write(addr + 1, u->Type);
	EEPROM.write(addr + 2, u->Pin);
	EEPROM.write(addr + 3, u->lhOn);
}

void Configuration::WriteNumberUnits() {
	EEPROM.write(addrNumberUnits, numberUnits);
}

void Configuration::ReadNumberUnits() {
	numberUnits = EEPROM.read(addrNumberUnits);
}


void Configuration::UpdateStorage() {
	for (int i = 0; i < numberUnits; i++) {
		Unit uROM;
		ReadUnit(i, &uROM);
		Debug2("id=", uROM.Id);
		if (!units[i]->compare(&uROM)) {
			Debug2("I=", i);
			WriteUnit(i, units[i]);
			//ReadUnit(i, &uROM);
			//Debug2("2id=", uROM.Id);
		}
	}
}