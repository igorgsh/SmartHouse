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
#include "OneWireBus.h"
#include "OneWireThermo.h"

extern Mqtt MqttClient;

void Configuration::CreateUnits() {
	if (units != NULL) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i] != NULL) {
				delete units[i];
			}
		}
		//free(units);
		delete units;
	}
	units = new Unit*[numberUnits];
	//units = (Unit**)malloc(numberUnits * sizeof(Unit*));

}

void Configuration::CreateActions() {
	if (actions != NULL) {
		for (int i = 0; i < numberActions; i++) {
			if (actions[i] != NULL) {
				delete actions[i];
			}
		}
		//free(actions);
		delete actions;
	}
	//actions = (Action**)malloc(numberActions * sizeof(Action*));
	actions = new Action*[numberActions];
}


Unit* Configuration::FindUnit(byte id) {
	
	if (units != NULL && IsConfigReady ) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Id == id) {
				return units[i];
			}
		}
	}
	return NULL;
}

Unit* Configuration::FindUnitByTypeAndPin(UnitType type, byte pin) {
	if (units != NULL /*&& IsConfigReady*/) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Type == type && units[i]->Pin == pin ) {
				return units[i];
			}
		}
	}
	return NULL;

}

void Configuration::MainLoop() {
	//	Step 1. Listening a server requests
	MqttClient.MqttLoop();
	// Step 2. Read all buttons
	Config.UnitsLoop();
}

void Configuration::InitializeServer() {

	if (Ethernet.begin(Config.mac) == 0) {
		Loger::Error("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		//if (Ethernet.begin(Config.mac, Config.ip) == 0) {
		//	SerialLog(D_ERROR, "Failed to configure Ethernet using DHCP");
		Config.IsEthernetConnection = false;
	}
	if (Config.IsEthernetConnection) {
		Loger::Info("Server is at " + String(Ethernet.localIP()));
	}
}


void Configuration::Init() {
	ReadBoardId();
	if (IsEthernetConnection) {
		Loger::Debug("Init Ethernet");
		InitializeServer();
		if (IsEthernetConnection) {
			Loger::Debug("Initialize MQTT");
			MqttClient.InitMqtt();
			MqttClient.SubscribeUnits();
		}
	}
	BuildConfig();
}

Unit* Configuration::CreateTypedUnit(byte type) {
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
			Loger::Error("Can't create Relay Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	else if (type == UnitType::ONE_WIRE_BUS) {
		u = new OneWireBus();
		if (u == NULL) {
			Loger::Error("Can't create OneWire bus Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_BUS;
	}
	else if (type == UnitType::ONE_WIRE_THERMO) {
		u = new OneWireThermo();
		if (u == NULL) {
			Loger::Error("Can't create OneWire Thermometer Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_THERMO;
	}
	return u;
}

void Configuration::ConvertStringToAddress(DeviceAddress address, const String addrStr) {
	for (int i = 0, j=0; i < 16; i+=2, j++) {
		unsigned long l = strtoul(addrStr.substring(i, i + 2).c_str(), NULL, 16);
		address[j] = l;
	}
}

String Configuration::ConvertAddressToString(const DeviceAddress address) {
	String str0 = "";

	for (int i = 0; i < 16; i ++) {
		str0 += String(address[i], HEX);
	}
	return str0;
}

void Configuration::UpdateConfig(const char *jsonConfig) {
	static bool lenDetected = false;
	StaticJsonBuffer<JSON_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
	if (root.containsKey("length")) {
		Loger::Debug("Length:" +String((int)root["length"]));

		int n = (int)root["length"];
		if (n != numberUnits) {
			numberUnits = n;
			WriteNumberUnits();
		}

		configCounter = 0;
		CreateUnits();
		lenDetected = true;
	}
	else if (lenDetected && root.containsKey("type") && root.containsKey("id")) {
		units[configCounter] = CreateTypedUnit(((const char*)root["type"])[0]);
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
			if (root.containsKey("address")) {
				if (units[configCounter]->Type == ONE_WIRE_THERMO) {
					ConvertStringToAddress(((OneWireBusUnit*)units[configCounter])->address, root["address"]);
				}
			}
		}

		units[configCounter]->print("Unit received", D_DEBUG);
		configCounter++;
		if (configCounter == numberUnits) {
			Loger::Debug("Finish update configuration");
			StoreUnits();
			IsConfigReady = true;
			IsServerConfig = true;

		}
		//Debug2("Memory6=", memoryFree());
	}
}

void Configuration::BuildConfig() {
	IsConfigReady = false;
	ReadNumberUnits();
	Loger::Debug("NumberOfunits=" + String(numberUnits));

	if (IsEthernetConnection) {
		MqttClient.GetConfiguration();
		unsigned long startServerRead = millis();
		while (!IsConfigReady && startServerRead + 10000 > millis()) { // we have a 10 seconds to get configuration
			MqttClient.MqttLoop();
		}
	}
	Loger::Debug("IsConfigReady=" +String(IsConfigReady));
	Loger::Debug("NumberOfunits=" + String(numberUnits));
	if (!IsConfigReady) { // We can't get config from server. Read from EEPROM
		IsServerConfig = false;
		Loger::Debug("Get From EEPROM");

		CreateUnits();
		for (int i = 0; i < numberUnits; i++) {
			UnitProto u0;
			ReadUnit(i, &u0);
			units[i] = CreateTypedUnit(u0.Type);
			units[i]->FillFrom(&u0);
			units[i]->SetDefault();

			units[i]->print("Unit From ROM:", D_DEBUG);
		}
		//Debug2("2NumberOfunits=", numberUnits);
		Loger::Debug("Done!");

	}
	BuildActions();
	InitializeUnits();
	FinalizeInitUnits();
	IsConfigReady = true;
}



void Configuration::InitializeUnits() {
	Loger::Debug("Init Units");
	for (int i = 0; i < numberUnits; i++) {
		units[i]->InitUnit();
	}

}

void Configuration::FinalizeInitUnits() {
	Loger::Debug("Finalize Init Units");
	for (int i = 0; i < numberUnits; i++) {
		units[i]->FinalInitUnit();
	}
}


void Configuration::InitializeActions() {
	Loger::Debug("Init Actions");
	for (int i = 0; i < numberActions; i++) {
		actions[i]->InitAction();
	}
}


void Configuration::ReadBoardId() {

	BoardId = EEPROM.read(addrBoardId);
	mac[3] = BoardId;
}

int Configuration::GetOneWireAddr(int ind) {
	return GetUnitsAddr(0) + numberUnits * sizeOfUnit + ind * sizeOfBusUnit;
}

void Configuration::ReadUnit(int ind, Unit* u) {
	/*
		Byte | Object
		-------------
		0: id
		1: type
		2: pin
		3: lhOn
	*/
	int addr = Configuration::addrUnits + ind * Configuration::sizeOfUnit;
	u->Id = EEPROM.read(addr);
	u->Type = EEPROM.read(addr + 1);
	u->Pin = EEPROM.read(addr + 2);
	u->lhOn = EEPROM.read(addr + 3);
	if (u->Type == UnitType::ONE_WIRE_THERMO) {
		OneWireBusUnit* owU = (OneWireBusUnit*)u;
		for (int i = 0; i < 8; i++) {
			owU->address[i] = EEPROM.read(GetOneWireAddr(ind) + i);
		}
	}

}

int Configuration::GetUnitsAddr(int i) {

	return Configuration::addrUnits + i * sizeOfUnit;

}

void Configuration::WriteUnit(int ind, const Unit* u) {
	/*
	Byte | Object
	-------------
	0: id
	1: type
	2: pin
	3: lhOn
	*/
	int addr = GetUnitsAddr(ind);
	EEPROM.write(addr, u->Id);
	EEPROM.write(addr + 1, u->Type);
	EEPROM.write(addr + 2, u->Pin);
	EEPROM.write(addr + 3, u->lhOn);
	if (u->Type == UnitType::ONE_WIRE_THERMO) {
		OneWireBusUnit* owU = (OneWireBusUnit*)u;
		for (int i = 0; i < 8; i++) {
			EEPROM.write(GetOneWireAddr(ind) + i, owU->address[i]);
		}
	}
}

void Configuration::WriteNumberUnits() {
	EEPROM.write(addrNumberUnits, numberUnits);
}

void Configuration::ReadNumberUnits() {
	numberUnits = EEPROM.read(addrNumberUnits);
}

void Configuration::ReadNumberBusUnits() {
	numberUnits = EEPROM.read(addrNumberBusUnits);
}
void Configuration::WriteNumberBusUnits() {
	EEPROM.write(addrNumberBusUnits, numberBusUnits);
}


void Configuration::StoreUnits() {
	for (int i = 0; i < numberUnits; i++) {
		UnitProto uROM;
		ReadUnit(i, &uROM);
		if (!units[i]->compare(&uROM)) {
			Loger::Debug("Write I=" +String(i));
			WriteUnit(i, units[i]);
		}
	}
}


void Configuration::UpdateActions(const char *jsonConfig) {
	//return;
	static bool lenDetected = false;
	StaticJsonBuffer<JSON_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
	//Debug("Update Actions");
	//root.prettyPrintTo(Serial);
	if (root.containsKey("length")) {
		Loger::Debug("Length:" +String((int)root["length"]));

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
		Loger::Debug("Action detected");
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

			actions[actionCounter]->print("Action received:", D_DEBUG);
		}

		actionCounter++;
		if (actionCounter == numberActions) {
			Loger::Debug("Finish update actions configuration");
			StoreActions();
			IsActionsReady = true;
			IsServerActions = true;

		}

	}
}
void Configuration::BuildActions() {
	IsActionsReady = false;
	ReadNumberActions();
	//Debug2("NumberOfActions=", numberActions);
	if (IsEthernetConnection) {
		MqttClient.GetActions();
		unsigned long startServerRead = millis();
		while (!IsActionsReady && startServerRead + 10000 > millis()) { // we have a 10 seconds to get actions
			MqttClient.MqttLoop();
		}
	}
	//Debug2("IsActionReady=", IsActionsReady);
	//Debug2("1NumberOfActions=", numberActions);
	if (!IsActionsReady) { // We can't get config from server. Read from EEPROM
		IsServerActions = false;
		Loger::Debug("Get Actions From EEPROM");
		CreateActions();
		for (int i = 0; i < numberActions; i++) {
			actions[i] = new Action();
			ReadAction(i, actions[i]);
		}
		//Debug2("2NumberOfActions=", numberActions);
		Loger::Debug("Actions Done!");
	}
	InitializeActions();
	IsActionsReady = true;
}

int Configuration::GetActionsAddr(int i) {
	return GetOneWireAddr(0) + numberBusUnits * sizeOfBusUnit + i * sizeOfAction;
}

void Configuration::ReadAction(int i, Action* a) {
	int addr = GetActionsAddr(i);
	//Debug2("Address Actions=", addr);
	a->Id = EEPROM.read(addr);
	a->originId = EEPROM.read(addr+1);
	a->originType = EEPROM.read(addr+2);
	a->event = EEPROM.read(addr+3);
	a->targetId = EEPROM.read(addr+4);
	a->targetAction = (ActionType)EEPROM.read(addr+5);
	a->print("Action From ROM:", D_DEBUG);
}

void Configuration::WriteAction(int i, const Action* a) {
	int addr = GetActionsAddr(i);
	//a->print("Action To ROM:", Serial);
	
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

		Loger::Debug("id=" + String(aROM.Id));
		if (!actions[i]->compare(&aROM)) {
			Loger::Debug("Write Actions I=" + String(i));
			WriteAction(i, actions[i]);
			ReadAction(i, &aROM);

		}
	}
}

void Configuration::UpdateUnit(UnitType type, String name, String value) {

	Unit *u = FindUnit(name.toInt());
	if (u != NULL) {
		u->ProcessUnit(value.toInt());
	}
}

void Configuration::UnitsLoop() {
	for (int i = 0; i < numberUnits; i++) {

		units[i]->UnitLoop();
	}
}


void Configuration::ProcessAction(byte id, byte event, unsigned long value) {


	for (int i = 0; i < numberActions; i++) {
		if (actions[i]->originId == id) {
			if (actions[i]->event == event) {
				Loger::Debug("Action Found!" + String(actions[i]->Id));
				actions[i]->print("ACTION:", D_DEBUG);
				Unit* originU = FindUnit(id);

				if (originU != NULL) {
					originU->print("Origin:",D_DEBUG);
					Unit* targetU = FindUnit(actions[i]->targetId);
					if (targetU != NULL) {
						targetU->print("Target: ", D_DEBUG);
			//			Debug2("Action target:", actions[i]->targetAction);
						switch (actions[i]->targetAction)
						{
						case ACT_NO_ACTION: {
							break;
						}
						case ACT_RELAY_SWITCH: {
							Loger::Debug("Relay Switch");
							if (targetU->Type == UnitType::RELAY) {
								((Relay*)targetU)->RelaySwitch();
							}
							else {
								Loger::Error("Action:" + String(actions[i]->Id) + ". Wrong type of target");
							}
							break;
						}
						case ACT_RELAY_ON: {
							if (targetU->Type == UnitType::RELAY) {
								((Relay*)targetU)->RelayOn();
							}
							else {
								Loger::Error("Action:" + String(actions[i]->Id) + ". Wrong type of target");
							}
							break;
						}
						case ACT_RELAY_OFF: {
							if (targetU->Type == UnitType::RELAY) {
								((Relay*)targetU)->RelayOff();
							}
							else {
								Loger::Error("Action:" + String(actions[i]->Id) + ". Wrong type of target");
							}
							break;
						}
						default:
							break;
						}
					}
					else {
						Loger::Error("Action:" + String(actions[i]->Id) + ". Target not found");
					}
				}
				else {
					Loger::Error("Action:" + String(actions[i]->Id) + ". Origin not found");
				}
			}
		}
	}
}
