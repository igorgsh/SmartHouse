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
#include "utils.h"

extern Mqtt MqttClient;

void Configuration::CreateUnits() {
	if (units != NULL) {
		for (int i = 0; i < numberUnits; i++) {
			delete units[i];
		}
		free(units);
	}
	units = (Unit**)malloc(numberUnits * sizeof(Unit*));

}

void Configuration::CreateActions() {
	if (actions != NULL) {
		for (int i = 0; i < numberActions; i++) {
			delete actions[i];
		}
		free(actions);
	}
	actions = (Action**)malloc(numberActions * sizeof(Action*));
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
		u = new Button();
		if (u == NULL) {
			Loger::Error("Can't create Button Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {
		u = new Relay();
		if (u == NULL) {
			Loger::Error("Can't create Button Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	return u;
}

void Configuration::UpdateConfig(const char *jsonConfig) {
	//return;
	Debug2("Point6.4.4.2:", memoryFree());

	static bool lenDetected = false;
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
	Debug("Update Config");
	//root.prettyPrintTo(Serial);
	Debug2("Point6.4.4.3:", memoryFree());
	if (root.containsKey("length")) {
		Debug2("Length:", (int)root["length"]);

		int n = (int)root["length"];
		Debug2("Point6.4.4.4:", memoryFree());
		if (n != numberUnits) {
			numberUnits = n;
			WriteNumberUnits();
		}
		Debug2("Point6.4.4.6:", memoryFree());

		configCounter = 0;
		CreateUnits();
		lenDetected = true;
		Debug2("Point6.4.4.8:", memoryFree());
	}
	else if (lenDetected && root.containsKey("type") && root.containsKey("id")) {
		Debug("Unit detected");
		Debug2("Point6.4.4.10:", memoryFree());

		units[configCounter] = CreateTypedUnit(((const char*)root["type"])[0]);
		Debug2("Point6.4.4.12:", memoryFree());

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
		Debug2("Point6.4.4.14:", memoryFree());

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
		Debug2("Point6.4.4.16:", memoryFree());

		if (configCounter == numberUnits) {
			Debug("Finish update configuration");
			StoreUnits();
			IsConfigReady = true;
			IsServerConfig = true;

		}
	}

}

void Configuration::BuildConfig() {
	IsConfigReady = false;
	ReadNumberUnits();
	Debug2("NumberOfunits=", numberUnits);
	Debug2("Point6.2:", memoryFree());

	MqttClient.GetConfiguration();
	unsigned long startServerRead = millis();
	Debug2("Point6.4:", memoryFree());
	while (!IsConfigReady && startServerRead+10000 > millis()) { // we have a 10 seconds to get configuration
		MqttClient.MqttLoop();
	}
	Debug2("Point6.6:", memoryFree());

	Debug2("IsConfigReady=", IsConfigReady);
	Debug2("1NumberOfunits=", numberUnits);
	if (!IsConfigReady) { // We can't get config from server. Read from EEPROM
		IsServerConfig = false;
		Debug("Get From EEPROM");

		CreateUnits();
		Debug2("Point6.8:", memoryFree());
		for (int i = 0; i < numberUnits; i++) {
			UnitProto u0;
			ReadUnit(i, &u0);
			units[i] = CreateTypedUnit(u0.Type);
			units[i]->FillFrom(&u0);
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
		Debug2("2NumberOfunits=", numberUnits);
		Debug("Done!");
		Debug2("Point6.10:", memoryFree());

	}
	Debug2("Point6.12:", memoryFree());

	InitializeUnits();
	Debug2("Point6.14:", memoryFree());

	BuildActions();
	Debug2("Point6.16:", memoryFree());

	IsConfigReady = true;
}



void Configuration::InitializeUnits() {
	// Initialize units
	Debug("Init Units");
	for (int i = 0; i < numberUnits; i++) {
		units[i]->InitUnit();
	}

}

void Configuration::InitializeActions() {
	// Initialize units
	Debug("Init Actions");
	for (int i = 0; i < numberActions; i++) {
		actions[i]->InitAction();
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
	//Debug2("Addr=", addr);
	u->Id = EEPROM.read(addr);
	//Debug2("u->Id=", u->Id);
	u->Type = EEPROM.read(addr + 1);
	//Debug2("u->Type=", u->Type);
	u->Pin = EEPROM.read(addr + 2);
	//Debug2("u->Pin=", u->Pin);
	u->lhOn = EEPROM.read(addr + 3);
	//Debug2("u->lhOn=", u->lhOn);

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


void Configuration::StoreUnits() {
	for (int i = 0; i < numberUnits; i++) {
		UnitProto uROM;
		ReadUnit(i, &uROM);
		Debug2("id=", uROM.Id);
		if (!units[i]->compare(&uROM)) {
			Debug2("Write I=", i);
			WriteUnit(i, units[i]);
			//ReadUnit(i, &uROM);
			//Debug2("2id=", uROM.Id);
		}
	}
}


void Configuration::UpdateActions(const char *jsonConfig) {
	//return;
	static bool lenDetected = false;
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
	Debug("Update Actions");
	//root.prettyPrintTo(Serial);
	if (root.containsKey("length")) {
		Debug2("Length:", (int)root["length"]);

		int n = (int)root["length"];
		if (n != numberActions) {
			numberActions = n;
			WriteNumberActions();
		}
		actionCounter = 0;
		CreateActions();
		lenDetected = true;
	}
	else if (lenDetected && root.containsKey("id")) {
		Debug("Action detected");
		actions[actionCounter] = new Action();
		//root.prettyPrintTo(Serial);
		if (actions[actionCounter] != NULL) {
			if (root.containsKey("id")) {
				actions[actionCounter]->Id = root["id"];
			}
			if (root.containsKey("originId")) {
				actions[actionCounter]->originId = root["originId"];
			}
			if (root.containsKey("originType")) {
				actions[actionCounter]->originType = (byte)(((const char*)root["originType"])[0]);
			}
			if (root.containsKey("event")) {
				actions[actionCounter]->event = root["event"];
			}
			if (root.containsKey("targetId")) {
				actions[actionCounter]->targetId = root["targetId"];
			}
			if (root.containsKey("targetAction")) {
				actions[actionCounter]->targetAction = (ActionType)((byte)root["targetAction"]);
			}

			actions[actionCounter]->print(Serial);
		}

		actionCounter++;
		if (actionCounter == numberActions) {
			Debug("Finish update actions configuration");
			StoreActions();
			IsActionsReady = true;
			IsServerActions = true;

		}
	}

}

void Configuration::BuildActions() {
	IsActionsReady = false;
	ReadNumberActions();
	Debug2("NumberOfActions=", numberActions);
	MqttClient.GetActions();
	unsigned long startServerRead = millis();
	while (!IsActionsReady && startServerRead + 10000 > millis()) { // we have a 10 seconds to get actions
		MqttClient.MqttLoop();
	}
	Debug2("IsActionReady=", IsActionsReady);
	Debug2("1NumberOfActions=", numberActions);
	if (!IsActionsReady) { // We can't get config from server. Read from EEPROM
		IsServerActions = false;
		Debug("Get Actions From EEPROM");
		CreateActions();
		for (int i = 0; i < numberActions; i++) {
			actions[i] = new Action();
			ReadAction(i, actions[i]);
		}
		Debug2("2NumberOfActions=", numberActions);
		Debug("Actions Done!");
	}
	InitializeActions();
	IsActionsReady = true;
}

int Configuration::GetActionsStartAddr() {
	return addrUnits + numberUnits * sizeOfUnit;
}

void Configuration::ReadAction(int i, Action* a) {
	int addr = GetActionsStartAddr() +  i * sizeOfAction;
	Debug2("Address Actions=", addr);
	a->Id = EEPROM.read(addr);
	a->originId = EEPROM.read(addr+1);
	a->originType = EEPROM.read(addr+2);
	a->event = EEPROM.read(addr+3);
	a->targetId = EEPROM.read(addr+4);
	a->targetAction = (ActionType)EEPROM.read(addr+5);
	Debug("Action From ROM:")
	a->print(Serial);
}

void Configuration::WriteAction(int i, const Action* a) {
	int addr = GetActionsStartAddr() + i * sizeOfAction;
	EEPROM.write(addr, a->Id);
	EEPROM.write(addr+1, a->originId);
	EEPROM.write(addr+2, a->originType);
	EEPROM.write(addr+3, a->event);
	EEPROM.write(addr+4, a->targetId);
	EEPROM.write(addr+5, a->targetAction);
}

void Configuration::WriteNumberActions() {
	EEPROM.write(addrNumberActions, numberActions);
}

void Configuration::ReadNumberActions() {
	numberActions = EEPROM.read(addrNumberActions);
}


void Configuration::StoreActions() {
	for (int i = 0; i < numberActions; i++) {
		Action aROM;
		ReadAction(i, &aROM);
		Debug2("id=", aROM.Id);
		if (!actions[i]->compare(&aROM)) {
			Debug2("Write Actions I=", i);
			WriteAction(i, actions[i]);
		}
	}
}

void Configuration::UpdateUnit(UnitType type, String name, String value) {

	Debug2("Name=", name);
	Debug2("Value=", value);
	Unit *u = FindUnit(name.toInt(), type);
	if (u != NULL) {
		//u->status = value.toInt();
		u->ProcessUnit(value.toInt());
	}
}
