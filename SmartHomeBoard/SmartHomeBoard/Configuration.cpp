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
//#include "Loger.h"
#include "utils.h"
#include "OneWireBus.h"
#include "OneWireThermo.h"
#include "PowerMeter.h"
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
<<<<<<< HEAD
	Loger::LogMessage = F("Create Units:");
	Loger::LogMessage += nUnits;
	Loger::Debug();
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	if (nUnits == 0) {
		units = NULL;
	}
	else {
		units = new Unit * [nUnits];
	}
	numberUnits = nUnits;
<<<<<<< HEAD
	return Config.units;
=======
	return units;
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
	Loger::LogMessage = F("Create Actions:");
	Loger::LogMessage += nActions;
	Loger::Debug();
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
	if (units != NULL /*&& IsConfigReady*/) {
=======
	if (units != NULL /*&& IsConfigReady*/ ) {
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		for (int i = 0; i < numberUnits; i++) {
			if (units[i]->Id == id) {
				return units[i];
			}
		}
	}
	return NULL;
}


void Configuration::MainLoop() {
	MqttClient->MqttLoop();
	UnitsLoop();
}

void Configuration::InitializeServer() {
<<<<<<< HEAD
	Ethernet.begin(Config.mac, Config.ip);
	Loger::LogMessage = F("Board IP is: ");
	PrintIP(Ethernet.localIP(), Loger::LogMessage);
	Loger::Info();
=======
	Ethernet.begin(mac, ip);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}


void Configuration::Init() {
	Log = new Loger(200);
	ReadBoardId();
	EthClient = new EthernetClient();
	
	if (IsEthernetConnection) {
<<<<<<< HEAD
		Loger::LogMessage = F("Init Ethernet");
		Loger::Debug();
		InitializeServer();
		Loger::LogMessage = F("Initialize MQTT");
		Loger::Debug();
		MqttClient.InitMqtt();
	}
	BuildConfig();
	BuildActions();
	Loger::LogMessage = F("Subscribe Units");
	Loger::Debug();
	MqttClient.SubscribeUnits();
	Loger::LogMessage = F("Config init is finished");
	Loger::Debug();
}


Configuration::Configuration()
{
//	BoardName.reserve(9);
// 	BoardName = "";
=======
		//Log->Debug(F1("Init Ethernet"));
		InitializeServer();
		//Log->Debug(F1("Initialize MQTT"));
		MqttClient = new Mqtt();
		MqttClient->InitMqtt();
	}
	BuildConfig();
	BuildActions();

	Log->Debug(F1("Subscribe Units"));
	MqttClient->SubscribeUnits();
	Log->Debug(F1("Config init is finished"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}

Unit* Configuration::CreateTypedUnit(byte type) {

	Unit* u = NULL;
	if (type == UnitType::BUTTON) {
		u = new Button();
		if (u == NULL) {
<<<<<<< HEAD
			Loger::LogMessage = F("Can't create Button Unit");
			Loger::Error();
=======
			Log->Error(F1("Can't create Button Unit"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {

		u = new Relay();
		if (u == NULL) {
<<<<<<< HEAD
			Loger::LogMessage = F("Can't create Relay Unit");
			Loger::Error();
=======
			Log->Error(F1("Can't create Relay Unit"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	else if (type == UnitType::ONE_WIRE_BUS) {
		u = new OneWireBus();
		if (u == NULL) {
<<<<<<< HEAD
			Loger::LogMessage = F("Can't create OneWire bus Unit");
			Loger::Error();
=======
			Log->Error(F1("Can't create OneWire bus Unit"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_BUS;
	}
	else if (type == UnitType::ONE_WIRE_THERMO) {
		u = new OneWireThermo();
		if (u == NULL) {
<<<<<<< HEAD
			Loger::LogMessage = F("Can't create OneWire Thermometer Unit");
			Loger::Error();
=======
			Log->Error(F1("Can't create OneWire Thermometer Unit"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_THERMO;
	}
	else if (type == UnitType::POWER_METER) {
		u = new PowerMeter();
		if (u == NULL) {
<<<<<<< HEAD
			Loger::LogMessage = F("Can't create Power Meter Unit");
			Loger::Error();
=======
			Log->Error(F1("Can't create Power Meter Unit"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			Board::Reset(10000);
		}
		u->Type = UnitType::POWER_METER;
	}
	else if (type == UnitType::CONTACTOR) {
		u = new Contactor();
		if (u == NULL) {
<<<<<<< HEAD
			Loger::LogMessage = F("Can't create Contactor");
			Loger::Error();
=======
			Log->Error(F1("Can't create Contactor"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			Board::Reset(10000);
		}
		u->Type = UnitType::CONTACTOR;
	}
	else if (type == UnitType::SHIFT_OUT) {
		u = new ShiftRegisterOut();
		if (u == NULL) {
			Loger::LogMessage = F("Can't create ShiftRegister Out");
			Loger::Error();
			Board::Reset(10000);
		}
		u->Type = UnitType::SHIFT_OUT;
	}
	else if (type == UnitType::SHIFT_IN) {
		u = new ShiftRegisterIn();
		if (u == NULL) {
			Loger::LogMessage = F("Can't create ShiftRegister In");
			Loger::Error();
			Board::Reset(10000);
		}
		u->Type = UnitType::SHIFT_OUT;
	}

	if (u == NULL) {
<<<<<<< HEAD
		Loger::LogMessage = F("Can't create a typed unit:");
		Loger::LogMessage += (char)type;
		Loger::Debug();
=======
		Log->append(F1("Can't create a typed unit:")).append((char)type).Error();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	}
	return u;
}


<<<<<<< HEAD
void Configuration::UpdateConfig(const String& jsonConfig) {
=======
void Configuration::UpdateConfig(const char* jsonConfig) {
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	if (isConfigRequested) {
		static bool lenDetected = false;
		const size_t CAPACITY = JSON_OBJECT_SIZE(JSON_SIZE);
		StaticJsonDocument<CAPACITY> doc;
		deserializeJson(doc, jsonConfig);

		// extract the data
		JsonObject root = doc.as<JsonObject>();

<<<<<<< HEAD
		if (root.containsKey(F("length"))) {
			byte nUnits = (byte)root[F("length")];
			Loger::LogMessage = F("Number of config Units=");
			Loger::LogMessage += numberUnits;
			Loger::Debug();
=======
		if (root.containsKey("length")) {
			byte nUnits = (byte)root["length"];
			Log->append(F1("Number of config Units=")).append(nUnits).Debug();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			configCounter = 0;
			CreateUnits(nUnits);
			lenDetected = true;
			if (nUnits == 0) {
				IsConfigReady = true;
			}
		}
		else if (lenDetected && root.containsKey(F("type")) && root.containsKey(F("id"))) {
			if (units != NULL) {
				units[configCounter] = CreateTypedUnit(((const char*)root[F("type")])[0]);
				if (units[configCounter] != NULL) {
					units[configCounter]->Id = root[F("id")];
					units[configCounter]->ConfigField(root);
				}

				configCounter++;
				if (configCounter == numberUnits) {
<<<<<<< HEAD
					Loger::LogMessage = F("Finish update configuration");
					Loger::Debug();
=======
					Log->append(F1("Finish update configuration(")).append(numberUnits).append(")").Debug();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
		while (!IsConfigReady && millis() - startLoop < MQTT_WAITING_RESPONSE) {
			MqttClient->MqttLoop();
		}
	}

	if (numberUnits != 0) {
		if (!IsConfigReady) { //Mqtt failed for some reasons
<<<<<<< HEAD
			Loger::LogMessage = F("Read Units from EEPROM");
			Loger::Debug();
=======
			Log->Debug(F1("Read Units from EEPROM"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			SigmaEEPROM::ReadUnits();
		}
		else {
			SigmaEEPROM::UpdateUnits(numberUnits, units);
		}
	} 
	IsConfigReady = true;
	InitializeUnits();
	FinalizeInitUnits();
<<<<<<< HEAD

	Loger::LogMessage = F("IsConfigReady=");
	Loger::LogMessage += IsConfigReady;
	Loger::Debug();
	Loger::LogMessage = F("NumberOfunits=");
	Loger::LogMessage += numberUnits;
	Loger::Debug();
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
	//BoardName.reserve(10);
	BoardName = F("Board_");
	BoardName += (BoardId < 10 ? "0" : "");
	BoardName += BoardId;

	mac[5] = BoardId;
	ip[3] = ip[3] + BoardId;

=======
	sprintf(BoardName, "Board_%02u", BoardId);
	mac[5] = BoardId;
	ip[3] = ip[3] + BoardId;

	sprintf(strIP, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}



<<<<<<< HEAD
void Configuration::UpdateActions(const String& jsonConfig) {
	if (isActionRequested) {
		static bool lenDetected = false;
=======
void Configuration::UpdateActions(const char* jsonConfig) {
	if (isActionRequested) {
		static bool lenDetected = false;
		// allocate the memory for the document
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		const size_t CAPACITY = JSON_OBJECT_SIZE(JSON_SIZE);
		StaticJsonDocument<CAPACITY> doc;
		deserializeJson(doc, jsonConfig);
		// extract the data
		JsonObject root = doc.as<JsonObject>();
<<<<<<< HEAD
		if (root.containsKey(F("length"))) {

			byte nActions = (byte)root[F("length")];
=======
		if (root.containsKey("length")) {
			byte nActions = (byte)root["length"];
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			actionCounter = 0;
			CreateActions(nActions);
			lenDetected = true;
		}
		else if (lenDetected && root.containsKey(F("id"))) {
			actions[actionCounter] = new Action();
			if (actions[actionCounter] != NULL) {
				if (root.containsKey(F("id"))) {
					actions[actionCounter]->Id = root[F("id")];
				}
				if (root.containsKey(F("originId"))) {
					actions[actionCounter]->originId = root[F("originId")];
				}
				if (root.containsKey(F("originType"))) {
					actions[actionCounter]->originType = (UnitType)(((const char*)root[F("originType")])[0]);
				}
				if (root.containsKey(F("event"))) {
					actions[actionCounter]->event = root[F("event")];
				}
				if (root.containsKey(F("targetId"))) {
					actions[actionCounter]->targetId = root[F("targetId")];
				}
				if (root.containsKey(F("targetAction"))) {
					actions[actionCounter]->targetAction = (ActionType)((byte)root[F("targetAction")]);
				}
				if (root.containsKey(F("targetType"))) {
					actions[actionCounter]->targetType = (UnitType)(((const char*)root[F("targetType")])[0]);
				}

			}

			actionCounter++;
			if (actionCounter == numberActions) {
<<<<<<< HEAD
				Loger::LogMessage = F("Finish update actions configuration");
				Loger::Debug();
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
		if (!IsActionsReady) { //Mqtt failed for some reasons
<<<<<<< HEAD
			Loger::LogMessage = F("Read Actions from EEPROM");
			Loger::Debug();
			SigmaEEPROM::ReadActions();
		}
		else {
			Loger::LogMessage = F("Update Actions(Build Actions)");
			Loger::Debug();
=======
			SigmaEEPROM::ReadActions();
		}
		else {
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			SigmaEEPROM::UpdateActions(numberActions, actions);
		}
	}

	InitializeActions();
	IsActionsReady = true;
<<<<<<< HEAD
	Loger::LogMessage = F("Actions are ready");
	Loger::Debug();
}

void Configuration::UpdateUnit(UnitType type, int id, const String& value) {

	Loger::LogMessage = F("Update Unit: ");
	Loger::LogMessage += id;

	Loger::Debug();

	Unit* u = FindUnit(id);
	if (u != NULL) {
		u->isSubscribed = true;
		u->ProcessUnit((ActionType)value.toInt());
	}
=======
}

void Configuration::UpdateUnit(UnitType type, uint16_t id, uint16_t value) {
	Unit *u = FindUnit(id);
	if (u != NULL) {
		//u->isSubscribed = true;
		u->ProcessUnit((ActionType)(value));
	} 
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}

void Configuration::UnitsLoop() {
	for (int i = 0; i < numberUnits; i++) {
		if (units[i]->Type != UnitType::POWER_METER) {
			units[i]->UnitLoop();
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
					originU->print("Origin:", D_DEBUG);
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
<<<<<<< HEAD
						Loger::LogMessage = F("Action:");
						Loger::LogMessage += actions[i]->Id;
						Loger::LogMessage += F(". Target not found");
						Loger::Error();
						Unit* u = new UnitProto();
=======
						Log->append(F1("Action:")).append(actions[i]->Id).append(F1(". Target not found")).Debug();
						Unit *u = new UnitProto();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
						u->Id = actions[i]->targetId;
						u->Type = actions[i]->targetType;
						u->status = actions[i]->targetAction;
						//u->isSubscribed = true; //this fake activation is used just for publish
						MqttClient->PublishUnit(u);
						delete u;
					}
				}
				else {
<<<<<<< HEAD
					Loger::LogMessage = F("Action:");
					Loger::LogMessage += actions[i]->Id;
					Loger::LogMessage += F(". Origin not found");
					Loger::Error();
=======
					Log->append(F1("Action:")).append(actions[i]->Id).append(F1(". Origin not found")).Error();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
				}
			}
		}
	}
}

void Configuration::loop1() {
};

void Configuration::loop30() {
};


void Configuration::loop10() {
	for (int i = 0; i < numberUnits; i++) {
		if (units[i]->Type == UnitType::POWER_METER) {
			((PowerMeter*)units[i])->PublishAll();
		}
	}
}

void Configuration::loop60() {
	// start every 1min
<<<<<<< HEAD
	MqttClient.WatchDog();
=======
	//Loger::Debug("Loop60");
	MqttClient->WatchDog();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}
