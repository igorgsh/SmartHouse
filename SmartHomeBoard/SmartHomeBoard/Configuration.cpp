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
		Loger::Debug("Point A1");
		for (int i = 0; i < numberUnits; i++) {
			Loger::Debug("Point A2");
			if (units[i] != NULL) {
				delete units[i];
			}
		}
		delete units;
	}
	Loger::Debug("Create Units:" + String(nUnits));
	units = new Unit*[nUnits];
	numberUnits = nUnits;
	//Loger::Debug("Number:" + String(numberUnits));
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
	Loger::Debug("Create Actions:" + String(nActions));
	actions = new Action*[nActions];
	numberActions = nActions;
	return actions;
}


Unit* Configuration::FindUnit(uint16_t id) {
	//Loger::Debug("Units=" + String(units != NULL) + "; Conf=" + String(IsConfigReady));
	if (units != NULL && IsConfigReady ) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Id == id) {
				return units[i];
			}
		}
	}
	return NULL;
}


void Configuration::MainLoop() {
	if (MqttClient.connected()) {
		MqttClient.MqttLoop();
	}
	Config.UnitsLoop();
}

void Configuration::InitializeServer() {
	Ethernet.begin(Config.mac, Config.ip);
	Loger::Info("Board IP is: " + PrintIP(Ethernet.localIP()));
}


void Configuration::Init() {
	ReadBoardId();
	if (IsEthernetConnection) {
		Loger::Debug("Init Ethernet");
		InitializeServer();
		Loger::Debug("Initialize MQTT");
		MqttClient.InitMqtt();
	}
	BuildConfig();
	BuildActions();

	if (MqttClient.connected()) {
		MqttClient.SubscribeUnits();
	}
	Loger::Debug("Config init is finished");
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
	else if (type == UnitType::POWER_METER) {
		u = new PowerMeter();
		if (u == NULL) {
			Loger::Error("Can't create Power Meter Unit");
			Board::Reset(10000);
		}
		u->Type = UnitType::POWER_METER;
	}
	else if (type == UnitType::CONTACTOR) {
		u = new Contactor();
		if (u == NULL) {
			Loger::Error("Can't create Contactor");
			Board::Reset(10000);
		}
		u->Type = UnitType::CONTACTOR;
	}
	if (u == NULL) {
		Loger::Debug("Can't create a typed unit:" + String((char)type));
	}
	return u;
}


void Configuration::UpdateConfig(String jsonConfig) {
	if (isConfigRequested) {
		static bool lenDetected = false;
		StaticJsonBuffer<JSON_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parse(jsonConfig);
		if (root.containsKey("length")) {
			byte nUnits = (byte)root["length"];
			Loger::Debug("Number of config Units=" + String(numberUnits));
			configCounter = 0;
			CreateUnits(nUnits);
			lenDetected = true;
		}
		else if (lenDetected && root.containsKey("type") && root.containsKey("id")) {

			units[configCounter] = CreateTypedUnit(((const char*)root["type"])[0]);
			if (units[configCounter] != NULL) {
				units[configCounter]->Id = root["id"];

				units[configCounter]->ConfigField(root);
				
			}

			configCounter++;
			if (configCounter == numberUnits) {
				Loger::Debug("Finish update configuration");
				IsConfigReady = true;
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


	if (!IsConfigReady) { //Mqtt failed for some reasons
		Loger::Debug("Read Units from EEPROM");
		SigmaEEPROM::ReadUnits();
	}
	else {
		//Loger::Debug("Update Units");
		SigmaEEPROM::UpdateUnits(numberUnits, units);
	}
	InitializeUnits();
	FinalizeInitUnits();
	IsConfigReady = true;

	Loger::Debug("IsConfigReady=" +String(IsConfigReady));
	Loger::Debug("NumberOfunits=" + String(numberUnits));
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

	BoardId = SigmaEEPROM::ReadBoardId();
	BoardName = "Board_" + (String)(BoardId < 10 ? "0" : "") + String(BoardId, DEC);
	Loger::Debug("BoardId=" + String(BoardId));
	mac[5] = BoardId;
	ip[3] = ip[3] + BoardId;
}



void Configuration::UpdateActions(String jsonConfig) {
	if (isActionRequested) {
		static bool lenDetected = false;
		StaticJsonBuffer<JSON_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parse(jsonConfig);
		if (root.containsKey("length")) {
			Loger::Debug("Length:" + String((int)root["length"]));

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
				Loger::Debug("Finish update actions configuration");
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

	if (!IsActionsReady) { //Mqtt failed for some reasons
		Loger::Debug("Read Actions from EEPROM");
		SigmaEEPROM::ReadActions();
	}
	else {
		Loger::Debug("Update Actions");
		SigmaEEPROM::UpdateActions(numberActions, actions);
	}


	InitializeActions();
	IsActionsReady = true;
	Loger::Debug("Actions are ready");
}

void Configuration::UpdateUnit(UnitType type, String name, String value) {
	Loger::Debug("Update Unit: " + name + "(" + String(name.toInt()) + ")");
	Unit *u = FindUnit(name.toInt());
	if (u != NULL) {
		Loger::Debug("Unit found:" + String(u->Id));
		u->isSubscribed = true;
		//Loger::Debug("Unit_type:" + String((char)u->Type));
		u->ProcessUnit((ActionType)(value.toInt()));
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
						Loger::Error("Action:" + String(actions[i]->Id) + ". Target not found");
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
					Loger::Error("Action:" + String(actions[i]->Id) + ". Origin not found");
				}
			}
		}
	}
}

void Configuration::loop30() {
	for (int i = 0; i < numberUnits; i++) {
		if (units[i]->Type == UnitType::POWER_METER) {
			Loger::Debug("Loop PowerMeter");
			((PowerMeter*)units[i])->PublishAll();
		}
	}
}

void Configuration::loop60() {
	// start every 1min
	//Loger::Debug("Loop60");
	MqttClient.WatchDog();
}
