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
//		Loger::Debug("Clean units:" + String(numberUnits));
		for (int i = 0; i < numberUnits; i++) {
//			Loger::Debug("Unit:" + String(i));
			if (units[i] != NULL) {
				delete units[i];
			}
		}
		delete units;
	}
	Loger::Debug("Create Units:" + String(numberUnits));
	units = new Unit*[numberUnits];

}

void Configuration::CreateActions() {
	if (actions != NULL) {
//		Loger::Debug("Clean Actions: " + String(numberActions));
		for (int i = 0; i < numberActions; i++) {
			if (actions[i] != NULL) {
				delete actions[i];
			}
		}
		delete actions;
	}
	Loger::Debug("Create Actions:" + String(numberActions));
	actions = new Action*[numberActions];
}


Unit* Configuration::FindUnit(uint16_t id) {
//	Loger::Debug("Units=" + String(units != NULL) + "; Conf=" + String(IsConfigReady));
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
	//Serial.println("MainLoop");
	//static unsigned long cnt = 0;
	
	//if (cnt % 10000 == 0) {
	//	Loger::Debug("Loop:" + String(cnt));
	//}
	MqttClient.MqttLoop();
	// Step 2. Read all buttons
	//Loger::Debug("Point 1");
	//if (cnt % 10000 == 0) {
	//	Loger::Debug("Units Loop:" + String(cnt));
	//}
	Config.UnitsLoop();
	//cnt++;
}

void Configuration::InitializeServer() {

	if (Ethernet.begin(Config.mac) == 0) {
		Loger::Error("Failed to configure Ethernet using DHCP");
		Config.IsEthernetConnection = false;
	}
	if (Config.IsEthernetConnection) {
		Loger::Info("Server is at " + PrintIP(Ethernet.localIP()));
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
		}
	}
	BuildConfig();
	if (IsEthernetConnection) {
		MqttClient.SubscribeUnits();
	}
}

Unit* Configuration::CreateTypedUnit(byte type) {

	Unit *u = NULL;
	//Loger::Debug("CreateTyped=" + String((char)type));
	if (type == UnitType::BUTTON) {
		//Loger::Debug("Point 8");
		u = new Button();
		if (u == NULL) {
			Loger::Error("Can't create Button Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {
		//Loger::Debug("Point 9");

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

void Configuration::UpdateConfig(String jsonConfig) {
	static bool lenDetected = false;
	StaticJsonBuffer<JSON_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
	if (root.containsKey("length")) {
		int n = (int)root["length"];
		Loger::Debug("Number of config Units=" + String(n));
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

		configCounter++;
		if (configCounter == numberUnits) {
			Loger::Debug("Finish update configuration");
			StoreUnits();
			IsConfigReady = true;
			IsServerConfig = true;

		}
	}
}

bool CheckConfigReady() {
	return Config.IsConfigReady;
}

void Configuration::BuildConfig() {
	IsConfigReady = false;
	ReadNumberUnits();
	Loger::Debug("NumberOfunits(ROM)=" + String(numberUnits));

	if (IsEthernetConnection) {
		MqttClient.GetConfiguration();
		unsigned long startLoop = millis();
		while (!IsConfigReady && millis() - startLoop < MQTT_WAITING_RESPONSE) {
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

			//units[i]->print("Unit From ROM:", D_DEBUG);
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
	BoardName = "Board_" + (String)(BoardId < 10 ? "0" : "") + String(BoardId, DEC);
	Loger::Debug("BoardId=" + String(BoardId));
	mac[5] = BoardId;
}

int Configuration::GetOneWireAddr(int ind) {
	return GetUnitsAddr(0) + numberUnits * sizeOfUnit + ind * sizeOfBusUnit;
}

void Configuration::ReadUnit(int ind, Unit* u) {
	/*
		Byte | Object
		-------------
		0-1: id
		2: type
		3: pin
		4: lhOn
	*/
	int addr = Configuration::addrUnits + ind * Configuration::sizeOfUnit;
	u->Id = Read16(addr);
	u->Type = EEPROM.read(addr + 2);
	u->Pin = EEPROM.read(addr + 3);
	u->lhOn = EEPROM.read(addr + 4);
	if (u->Type == UnitType::ONE_WIRE_THERMO) {
		OneWireBusUnit* owU = (OneWireBusUnit*)u;
		for (int i = 0; i < 8; i++) {
			owU->address[i] = EEPROM.read(GetOneWireAddr(ind) + i);
		}
	}
//	u->print("Unit from ROM:", D_DEBUG);
}

int Configuration::GetUnitsAddr(int i) {

	return Configuration::addrUnits + i * sizeOfUnit;

}

void Configuration::WriteUnit(int ind, const Unit* u) {
	/*
	Byte | Object
	-------------
	0-1: id
	2: type
	3: pin
	4: lhOn
	*/
	int addr = GetUnitsAddr(ind);
	Write16(addr, u->Id);
	EEPROM.write(addr + 2, u->Type);
	EEPROM.write(addr + 3, u->Pin);
	EEPROM.write(addr + 4, u->lhOn);
	if (u->Type == UnitType::ONE_WIRE_THERMO) {
		OneWireBusUnit* owU = (OneWireBusUnit*)u;
		for (int i = 0; i < 8; i++) {
			EEPROM.write(GetOneWireAddr(ind) + i, owU->address[i]);
		}
	}
//	u->print("Unit TO ROM:", D_DEBUG);
}

void Configuration::WriteNumberUnits() {
	EEPROM.write(addrNumberUnits, numberUnits);
}

void Configuration::ReadNumberUnits() {
	numberUnits = EEPROM.read(addrNumberUnits);
	numberUnits = (numberUnits == 255 ? 0 : numberUnits);
	//Loger::Debug(String(numberUnits));
}

void Configuration::ReadNumberBusUnits() {
	numberBusUnits = EEPROM.read(addrNumberBusUnits);
	numberBusUnits = (numberBusUnits == 255 ? 0 : numberBusUnits);
}
void Configuration::WriteNumberBusUnits() {
	EEPROM.write(addrNumberBusUnits, numberBusUnits);
}


void Configuration::StoreUnits() {
	bool isUpdated = false;
	for (int i = 0; i < numberUnits; i++) {
		UnitProto uROM;
		ReadUnit(i, &uROM);
		if (!units[i]->compare(&uROM)) {
			Loger::Debug("Write I=" +String(i));
			WriteUnit(i, units[i]);
			isUpdated = true;
		}
	}
	if (isUpdated) {
		MqttClient.SubscribeUnits();
	}
}


void Configuration::UpdateActions(String jsonConfig) {
	static bool lenDetected = false;
	StaticJsonBuffer<JSON_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parse(jsonConfig);
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
		actions[actionCounter] = new Action();
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
			if (root.containsKey("targetType")) {
				actions[actionCounter]->targetType = (byte)(((const char*)root["targetType"])[0]);
			}

			//actions[actionCounter]->print("Action received:", D_DEBUG);
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

bool CheckActions() {
	return Config.IsActionsReady;
}

void Configuration::BuildActions() {
	IsActionsReady = false;
	ReadNumberActions();
	Loger::Debug("NumberOfActions(ROM)=" + String(numberActions));
	if (IsEthernetConnection) {
		MqttClient.GetActions();
		unsigned long startLoop = millis();
		while (!IsActionsReady && millis() - startLoop < MQTT_WAITING_RESPONSE ) {
			MqttClient.MqttLoop();
		}
	}
	if (!IsActionsReady) { // We can't get config from server. Read from EEPROM
		IsServerActions = false;
		Loger::Debug("Get Actions From EEPROM");
		CreateActions();
		for (int i = 0; i < numberActions; i++) {
			actions[i] = new Action();
			ReadAction(i, actions[i]);
		}
		Loger::Debug("Actions Done!");
	}
	InitializeActions();
	IsActionsReady = true;
}

int Configuration::GetActionsAddr(int i) {
	return GetOneWireAddr(0) + numberBusUnits * sizeOfBusUnit + i * sizeOfAction;
}

uint16_t Configuration::Read16(uint16_t addr) {
	uint16_t res;
	res = (((uint16_t)EEPROM.read(addr)) << 8 & 0xFF00) + EEPROM.read(addr + 1);
	return res;
}

void Configuration::Write16(uint16_t addr, uint16_t val) {
	EEPROM.write(addr, (byte)((val >> 8) & 0x00FF));
	EEPROM.write(addr+1, (byte)((val & 0x00FF)));
}


void Configuration::ReadAction(int i, Action* a) {
	int addr = GetActionsAddr(i);
	a->Id = Read16(addr);
	a->originId = Read16(addr+2);
	a->originType = EEPROM.read(addr+4);
	a->event = EEPROM.read(addr+5);
	a->targetId = Read16(addr+6);
	a->targetAction = (ActionType)EEPROM.read(addr+8);
	a->targetType = EEPROM.read(addr + 9);
	a->print("Action From ROM:", D_DEBUG);
}

void Configuration::WriteAction(int i, const Action* a) {
	int addr = GetActionsAddr(i);
	//a->print("Action To ROM:", Serial);
	
	Write16(addr, a->Id);
	Write16(addr+2, a->originId);
	EEPROM.write(addr+4, a->originType);
	EEPROM.write(addr+5, a->event);
	Write16(addr+6, a->targetId);
	EEPROM.write(addr+8, a->targetAction);
	EEPROM.write(addr + 9, a->targetType);
}

void Configuration::WriteNumberActions() {
	EEPROM.write(addrNumberActions, numberActions);
}

void Configuration::ReadNumberActions() {
	numberActions = EEPROM.read(addrNumberActions);
	numberActions = (numberActions == 255 ? 0 : numberActions);

}


void Configuration::StoreActions() {
	for (int i = 0; i < numberActions; i++) {
		Action aROM;
		ReadAction(i, &aROM);

		//Loger::Debug("id=" + String(aROM.Id));
		if (!actions[i]->compare(&aROM)) {
			Loger::Debug("Write Actions I=" + String(i));
			WriteAction(i, actions[i]);
			//ReadAction(i, &aROM);

		}
	}
}

void Configuration::UpdateUnit(UnitType type, String name, String value) {
	Loger::Debug("Update Unit: " + name + "(" + String(name.toInt()) + ")");
	MEMFREE;
	Unit *u = FindUnit(name.toInt());
	if (u != NULL) {
		Loger::Debug("Unit found:" + String(u->Id));
		u->ProcessUnit((ActionType)(value.toInt()));
	} 
	//Loger::Debug("End Update Unit");

}

void Configuration::UnitsLoop() {
	for (int i = 0; i < numberUnits; i++) {
		units[i]->UnitLoop();
	}
}


void Configuration::ProcessAction(uint16_t id, byte event) {


	for (int i = 0; i < numberActions; i++) {
		if (actions[i]->originId == id) {
			if (actions[i]->event == event) {
				//Loger::Debug("Action Found!" + String(actions[i]->Id));
				actions[i]->print("ACTION:", D_DEBUG);
				Unit* originU = FindUnit(id);

				if (originU != NULL) {
					originU->print("Origin:",D_DEBUG);
					Unit* targetU = FindUnit(actions[i]->targetId);
					if (targetU != NULL) {
						targetU->print("Target: ", D_DEBUG);

						switch (actions[i]->targetType) {
						case UnitType::RELAY: {
							((Relay*)targetU)->ProcessUnit(actions[i]->targetAction);
							break;
						}
						default:
							break;
						}
					}
					else {
						Loger::Error("Action:" + String(actions[i]->Id) + ". Target not found");
						Unit *u = new UnitProto();
						u->Id = actions[i]->targetId;
						u->Type = actions[i]->targetType;
						u->status = actions[i]->targetAction;
						MqttClient.PublishUnit(u);
					}
				}
				else {
					Loger::Error("Action:" + String(actions[i]->Id) + ". Origin not found");
				}
			}
		}
	}
}

void Configuration::loop01() {
	// start every 100 ms
}

void Configuration::loop05() {
	// start every 500 ms
}
void Configuration::loop1() {
	Loger::Debug("Loop1");
	Loger::Debug("Counter01=" + String(Config.counter01,DEC));
	MEMFREE
	// start every 1s
}
void Configuration::loop60() {
	// start every 1min
	//Loger::Debug("Loop60");
	MqttClient.WatchDog();
}
void Configuration::loop300() {
	// start every 5 min
}

