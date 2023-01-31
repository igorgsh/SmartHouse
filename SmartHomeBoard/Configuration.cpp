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
#include "VoltmeterDC.h"
#include "Contactor.h"
#include "ShiftRegisterOut.h"
#include "ShiftRegisterIn.h"

//extern Mqtt MqttClient;

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
		units = new Unit* [nUnits];
	}
	numberUnits = nUnits;
	return units;
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

void Configuration::UpdateCommonParams(const char* topic, const char* payload)
{
	if (strcmp(topic, MQTT_DELAY_OFF_6) == 0) {
		Delay_OFF_6 = atol(payload);
	} else if (strcmp(topic, MQTT_DELAY_ON_7) == 0) {
		Delay_ON_7 = atol(payload);
	}
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


void Configuration::Loop(unsigned long timePeriod) {
	if (timePeriod == 0) {
		MqttClient->MqttLoop();
		UnitsLoop(timePeriod);
	}
	else if (timePeriod == 60000) {
		UnitsLoop(timePeriod);
		MqttClient->WatchDog();
	}
	else if (timePeriod == 30000) {
		UnitsLoop(timePeriod);
	}
	else if (timePeriod == 10000) {
		UnitsLoop(timePeriod);
	}
	else if (timePeriod == 1000) {
		UnitsLoop(timePeriod);
	}
}

void Configuration::InitializeServer() {
	Ethernet.begin(mac, ip);
}


void Configuration::Init() {
	Log = new Loger(200);
	ReadBoardId();
	EthClient = new EthernetClient();
	
	if (IsEthernetConnection) {
		InitializeServer();
		MqttClient = new Mqtt();
		MqttClient->InitMqtt();
	}
	BuildConfig();
	BuildActions();

//	Log->Info(F("Subscribe Units"));
//	MqttClient->SubscribeUnits();
	Log->Info(F("Config init is finished"));
}

Unit* Configuration::CreateTypedUnit(byte type) {

	Unit *u = NULL;
	if (type == UnitType::BUTTON) {
		u = new Button();
		if (u == NULL) {
			Log->Error(F("Can't create Button Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {

		u = new Relay();
		if (u == NULL) {
			Log->Error(F("Can't create Relay Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	//else if (type == UnitType::ONE_WIRE_BUS) {
	//	u = new OneWireBus();
	//	if (u == NULL) {
	//		Log->Error(F("Can't create OneWire bus Unit"));
	//		Board::Reset(10000);
	//	}
	//	u->Type = UnitType::ONE_WIRE_BUS;
	//}
	//else if (type == UnitType::ONE_WIRE_THERMO) {
	//	u = new OneWireThermo();
	//	if (u == NULL) {
	//		Log->Error(F("Can't create OneWire Thermometer Unit"));
	//		Board::Reset(10000);
	//	}
	//	u->Type = UnitType::ONE_WIRE_THERMO;
	//}
	else if (type == UnitType::POWER_METER) {
		u = new PowerMeter();
		if (u == NULL) {
			Log->Error(F("Can't create Power Meter Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::POWER_METER;
	}
	else if (type == UnitType::CONTACTOR) {
		u = new Contactor();
		if (u == NULL) {
			Log->Error(F("Can't create Contactor"));
			Board::Reset(10000);
		}
		u->Type = UnitType::CONTACTOR;
	} 
	else if (type == UnitType::SHIFTOUT) {
		u = new ShiftRegisterOut();
		if (u == NULL) {
			Log->Error(F("Can't create ShiftRegisterOut"));
			Board::Reset(10000);
		}
		u->Type = UnitType::SHIFTOUT;
	}
	else if (type == UnitType::SHIFTIN) {
		u = new ShiftRegisterIn();
		if (u == NULL) {
			Log->Error(F("Can't create ShiftRegisterOut"));
			Board::Reset(10000);
		}
		u->Type = UnitType::SHIFTIN;
	}
	else if (type == UnitType::VOLTMETER) {
		u = new VoltmeterDC();
		if (u == NULL) {
			Log->Error(F("Can't create Voltmeter Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::VOLTMETER;
	}
	if (u == NULL) {
		Log->append(F("Can't create a typed unit:")).append((char)type).Error();
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
			Log->append(F("Number of config Units=")).append(nUnits).Info();
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
					Log->append(F("Finish update configuration(")).append(numberUnits).append(")").Info();
					IsConfigReady = true;
				}
			}
		}
	}
}

//bool CheckConfigReady() {
//	return IsConfigReady;
//}

void Configuration::BuildConfig() {
	IsConfigReady = false;

	if (MqttClient->connected()) {
		MqttClient->GetConfiguration();
		unsigned long startLoop = millis();
		while (!IsConfigReady && (millis() - startLoop < MQTT_WAITING_RESPONSE)) {
			MqttClient->MqttLoop();
		}
	}

	if (numberUnits != 0) {
		if (IsConfigReady) { //Mqtt is ok
			SigmaEEPROM::UpdateUnits(numberUnits, units);
		}
	} else { // mqtt failed
		Log->Info(F("Read Units from EEPROM"));
		SigmaEEPROM::ReadUnits();
	}
	IsConfigReady = true;
	InitializeUnits();
	FinalizeInitUnits();

	//request common params
	MqttClient->Subscribe(MQTT_DELAY_OFF_6);
	MqttClient->Subscribe(MQTT_DELAY_ON_7);

	MqttClient->Publish(MQTT_STATE_REQUEST, MQTT_DELAY_OFF_6);
	MqttClient->Publish(MQTT_STATE_REQUEST, MQTT_DELAY_ON_7);
}



void Configuration::InitializeUnits() {
	for (int i = 0; i < numberUnits; i++) {
		if (units[i]->parentId == 0) {
			units[i]->InitUnit();
		}
	}

}

void Configuration::FinalizeInitUnits() {
	for (int i = 0; i < numberUnits; i++) {
		if (units[i]->parentId == 0) {
			units[i]->FinalInitUnit();
		}
	}
}


void Configuration::InitializeActions() {
	for (int i = 0; i < numberActions; i++) {
		actions[i]->InitAction();
	}
}


void Configuration::ReadBoardId() {
	SigmaEEPROM::ReadBoardId();
	sprintf(BoardName, "Board_%02u", BoardId);
	mac[5] = BoardId;
	mqttIp[0] = ip[0];
	mqttIp[1] = ip[1];
	mqttIp[2] = ip[2];

	Log->append(F("IP Address is:")).append(ip[0]).append(".").append(ip[1]).append(".")
		.append(ip[2]).append(".").append(ip[3]).Info();
	Log->append(F("Mqtt Server IP is:")).append(mqttIp[0]).append(".").append(mqttIp[1]).append(".")
		.append(mqttIp[2]).append(".").append(mqttIp[3]).Info();

	//ip[3] = ip[3] + BoardId;

	//sprintf(strIP, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
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
			lenDetected = true;
			if (nActions == 0) {
				IsActionsReady = true;
			}
			else {
				CreateActions(nActions);
			}
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

//bool CheckActions() {
//	return Config.IsActionsReady;
//}




void Configuration::BuildActions() {
	IsActionsReady = false;
	if (MqttClient->connected()) {
		MqttClient->GetActions();
		unsigned long startLoop = millis();
		while (!IsActionsReady && millis() - startLoop < MQTT_WAITING_RESPONSE) {
			MqttClient->MqttLoop();
		}
	}

	if (numberActions != 0) {
		if (IsActionsReady) { //Mqtt is ok
			SigmaEEPROM::UpdateActions(numberActions, actions);
		}
	}
	else { // mqtt failed
		Log->Info(F("Read Actions from EEPROM"));
		SigmaEEPROM::ReadActions();
	}

	InitializeActions();
	IsActionsReady = true;
}

void Configuration::UpdateUnit(UnitType type, uint16_t id, uint16_t value) {
	Unit *u = FindUnit(id);
	if (u != NULL) {
		//u->isSubscribed = true;
		u->ProcessUnit((ActionType)(value));
	} 
}

void Configuration::UnitsLoop(unsigned long timePeriod){
	for (int i = 0; i < numberUnits; i++) {
		units[i]->UnitLoop(timePeriod);
	}
}

void Configuration::Pass2Parent(uint16_t parentId, byte parentPin, bool status) {

	Unit* parent = FindUnit(parentId);
	if (parent != NULL) {
		if (parent->Type == UnitType::SHIFTOUT) {
			ShiftRegisterOut* SRegOut = (ShiftRegisterOut*)parent;
			SRegOut->Set(parentPin, status);
		}
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
						Unit::PublishTypedUnit(actions[i]->targetType, actions[i]->targetId, actions[i]->targetAction);
					}
				}
				else {
					Log->append(F("Action:")).append(actions[i]->Id).append(F(". Origin not found")).Error();
				}
			}
		}
	}
}
