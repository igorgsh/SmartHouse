// 
// 
// 

#include "configuration.h"
#include "mqtt.h"
#include <ArduinoJson.h>
#include "button.h"
#include "relay.h"
//#include "EEPROM.h"
#include "Unit.h"
#include "Board.h"
#include "Loger.h"
#include "utils.h"
#include "OneWireBus.h"
#include "OneWireThermo.h"
#include "PowerMeter.h"
#include "Contactor.h"

extern Mqtt MqttClient;

Unit** Configuration::CreateUnits(byte nUnits) {

	if (units != NULL) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i] != NULL) {
				delete units[i];
			}
		}
		delete units;
	}
	if (nUnits == 0) {
		units = NULL;
	}
	else {
		units = new Unit * [nUnits];
	}
	numberUnits = nUnits;
	return Config.units;
}

Action** Configuration::CreateActions(byte nActions) {
	if (actions != NULL) {
		for (int i = 0; i < nActions; i++) {
			if (actions[i] != NULL) {
				delete actions[i];
			}
		}
		delete actions;
	}
	if (nActions == 0) {
		actions = NULL;
	}
	else {
		actions = new Action * [nActions];
	}
	numberActions = nActions;
	return actions;
}


Unit* Configuration::FindUnit(uint16_t id) {
	if (units != NULL /*&& IsConfigReady*/ ) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Id == id) {
				return units[i];
			}
		}
	}
	return NULL;
}


void Configuration::MainLoop() {
	MqttClient.MqttLoop();
	Config.UnitsLoop();
}

void Configuration::InitializeServer() {
	Ethernet.begin(Config.mac, Config.ip);
}


void Configuration::Init() {
	ReadBoardId();
	if (IsEthernetConnection) {
		Log.Debug(F("Init Ethernet"));
		InitializeServer();
		Log.Debug(F("Initialize MQTT"));
		MqttClient.InitMqtt();
	}
	BuildConfig();
	BuildActions();

	Log.Debug(F("Subscribe Units"));
	MqttClient.SubscribeUnits();
	Log.Debug(F("Config init is finished"));
}

Unit* Configuration::CreateTypedUnit(byte type) {

	Unit *u = NULL;
	if (type == UnitType::BUTTON) {
		u = new Button();
		if (u == NULL) {
			Log.Error(F("Can't create Button Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {

		u = new Relay();
		if (u == NULL) {
			Log.Error(F("Can't create Relay Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	else if (type == UnitType::ONE_WIRE_BUS) {
		u = new OneWireBus();
		if (u == NULL) {
			Log.Error(F("Can't create OneWire bus Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_BUS;
	}
	else if (type == UnitType::ONE_WIRE_THERMO) {
		u = new OneWireThermo();
		if (u == NULL) {
			Log.Error(F("Can't create OneWire Thermometer Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_THERMO;
	}
	else if (type == UnitType::POWER_METER) {
		u = new PowerMeter();
		if (u == NULL) {
			Log.Error(F("Can't create Power Meter Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::POWER_METER;
	}
	else if (type == UnitType::CONTACTOR) {
		u = new Contactor();
		if (u == NULL) {
			Log.Error(F("Can't create Contactor"));
			Board::Reset(10000);
		}
		u->Type = UnitType::CONTACTOR;
	}
	if (u == NULL) {
		Log.append(F("Can't create a typed unit:")).append((char)type).Error();
	}
	return u;
}


void Configuration::UpdateConfig(const char* jsonConfig) {
	if (isConfigRequested) {
		static bool lenDetected = false;
		const size_t CAPACITY = JSON_OBJECT_SIZE(JSON_SIZE);
		StaticJsonDocument<CAPACITY> doc;
		deserializeJson(doc, jsonConfig);
		// extract the data
		JsonObject root = doc.as<JsonObject>();

		if (root.containsKey("length")) {
			byte nUnits = (byte)root["length"];
			Log.append(F("Number of config Units=")).append(numberUnits).Debug();
			configCounter = 0;
			CreateUnits(nUnits);
			lenDetected = true;
			if (nUnits == 0) {
				IsConfigReady = true;
			}

		}
		else if (lenDetected && root.containsKey("type") && root.containsKey("id")) {
			if (units != NULL) {

				units[configCounter] = CreateTypedUnit(((const char*)root["type"])[0]);
				if (units[configCounter] != NULL) {
					units[configCounter]->Id = root["id"];

					units[configCounter]->ConfigField(root);

				}

				configCounter++;
				if (configCounter == numberUnits) {
					Log.Debug(F("Finish update configuration"));
					IsConfigReady = true;
				}
			}
		}
	}
}

bool CheckConfigReady() {
	return Config.IsConfigReady;
}

void Configuration::BuildConfig() {
	IsConfigReady = false;

	if (MqttClient.connected()) {
		MqttClient.GetConfiguration();
		unsigned long startLoop = millis();
		while (!IsConfigReady && millis() - startLoop < MQTT_WAITING_RESPONSE) {
			MqttClient.MqttLoop();
		}
	}

	if (numberUnits != 0) {
		if (!IsConfigReady) { //Mqtt failed for some reasons
			Log.Debug(F("Read Units from EEPROM"));
			SigmaEEPROM::ReadUnits();
		}
		else {
			SigmaEEPROM::UpdateUnits(numberUnits, units);
		}
	}
	IsConfigReady = true;
	InitializeUnits();
	FinalizeInitUnits();
}



void Configuration::InitializeUnits() {
	for (int i = 0; i < numberUnits; i++) {
		units[i]->InitUnit();
	}

}

void Configuration::FinalizeInitUnits() {
	for (int i = 0; i < numberUnits; i++) {
		units[i]->FinalInitUnit();
	}
}


void Configuration::InitializeActions() {
	for (int i = 0; i < numberActions; i++) {
		actions[i]->InitAction();
	}
}


void Configuration::ReadBoardId() {

	BoardId = SigmaEEPROM::ReadBoardId();
	sprintf(BoardName, "Board_%02u", BoardId);
	mac[5] = BoardId;
	ip[3] = ip[3] + BoardId;
	sprintf(strIP, "%3u.%3u.%3u.%3u", ip[0], ip[1], ip[2], ip[3]);
}



void Configuration::UpdateActions(const char* jsonConfig) {
	if (isActionRequested) {
		static bool lenDetected = false;
		// allocate the memory for the document
		const size_t CAPACITY = JSON_OBJECT_SIZE(JSON_SIZE);
		StaticJsonDocument<CAPACITY> doc;
		deserializeJson(doc, jsonConfig);
		// extract the data
		JsonObject root = doc.as<JsonObject>();
		if (root.containsKey("length")) {
			byte nActions = (byte)root["length"];
			actionCounter = 0;
			CreateActions(nActions);
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
					actions[actionCounter]->originType = (UnitType)(((const char*)root["originType"])[0]);
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
					actions[actionCounter]->targetType = (UnitType)(((const char*)root["targetType"])[0]);
				}

			}

			actionCounter++;
			if (actionCounter == numberActions) {
				IsActionsReady = true;
			}
		}
	}
}

bool CheckActions() {
	return Config.IsActionsReady;
}




void Configuration::BuildActions() {
	IsActionsReady = false;
	if (MqttClient.connected()) {
		MqttClient.GetActions();
		unsigned long startLoop = millis();
		while (!IsActionsReady && millis() - startLoop < MQTT_WAITING_RESPONSE) {
			MqttClient.MqttLoop();
		}
	}
	if (numberActions != 0) {
		if (!IsActionsReady) { //Mqtt failed for some reasons
			SigmaEEPROM::ReadActions();
		}
		else {
			SigmaEEPROM::UpdateActions(numberActions, actions);
		}
	}

	InitializeActions();
	IsActionsReady = true;
}

void Configuration::UpdateUnit(UnitType type, uint16_t id, uint16_t value) {
	Unit *u = FindUnit(id);
	if (u != NULL) {
		u->isSubscribed = true;
		u->ProcessUnit((ActionType)(value));
	} 
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
						Log.append(F("Action:")).append(actions[i]->Id).append(F(". Target not found")).Debug();
						Unit *u = new UnitProto();
						u->Id = actions[i]->targetId;
						u->Type = actions[i]->targetType;
						u->status = actions[i]->targetAction;
						u->isSubscribed = true; //this fake activation is used just for publish
						MqttClient.PublishUnit(u);
						delete u;
					}
				}
				else {
					Log.append(F("Action:")).append(actions[i]->Id).append(F(". Origin not found")).Error();
				}
			}
		}
	}
}

void Configuration::loop30() {
	for (int i = 0; i < numberUnits; i++) {
		if (units[i]->Type == UnitType::POWER_METER) {
			((PowerMeter*)units[i])->PublishAll();
		}
	}
}

void Configuration::loop60() {
	// start every 1min
	//Loger::Debug("Loop60");
	MqttClient.WatchDog();
}
