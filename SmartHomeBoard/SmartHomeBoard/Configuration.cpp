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
#include "PowerMeter.h"

extern Mqtt MqttClient;

void Configuration::CreateUnits() {
	if (units != NULL) {
		for (int i = 0; i < numberUnits; i++) {
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
	Loger::Debug("Units=" + String(units != NULL) + "; Conf=" + String(IsConfigReady));
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
	if (units != NULL) {
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Type == type && units[i]->Pin == pin ) {
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
	if (isConfigRequested) {
		static bool lenDetected = false;
		StaticJsonBuffer<JSON_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parse(jsonConfig);
		if (root.containsKey("length")) {
			numberUnits = (int)root["length"];
			Loger::Debug("Number of config Units=" + String(numberUnits));
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
				if (root.containsKey("Serial")) {
					if (units[configCounter]->Type == POWER_METER) {
						((PowerMeter*)units[configCounter])->serialNumber = root["Serial"];
					}
				}
				if (root.containsKey("SerialRX")) {
					if (units[configCounter]->Type == POWER_METER) {
						((PowerMeter*)units[configCounter])->serialRX = root["SerialRX"];
					}
				}
				if (root.containsKey("SerialTX")) {
					if (units[configCounter]->Type == POWER_METER) {
						((PowerMeter*)units[configCounter])->serialTX = root["SerialTX"];
					}
				}
				if (root.containsKey("Factor")) {
					if (units[configCounter]->Type == POWER_METER) {
						((PowerMeter*)units[configCounter])->factor = root["Factor"];
					}
				}
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

	if (IsEthernetConnection) {
		MqttClient.GetConfiguration();
		unsigned long startLoop = millis();
		while (!IsConfigReady && millis() - startLoop < MQTT_WAITING_RESPONSE) {
			MqttClient.MqttLoop();
		}
	}
	Loger::Debug("IsConfigReady=" +String(IsConfigReady));
	Loger::Debug("NumberOfunits=" + String(numberUnits));
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



void Configuration::UpdateActions(String jsonConfig) {
	if (isActionRequested) {
		static bool lenDetected = false;
		StaticJsonBuffer<JSON_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parse(jsonConfig);
		if (root.containsKey("length")) {
			Loger::Debug("Length:" + String((int)root["length"]));

			numberActions = (int)root["length"];
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
	MqttClient.GetActions();
	unsigned long startLoop = millis();
	while (!IsActionsReady && millis() - startLoop < MQTT_WAITING_RESPONSE ) {
		MqttClient.MqttLoop();
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
		if (!u->isSubscribed) {
			Loger::Debug("Unit's subscription activated");
			u->isSubscribed = true;
		}
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
