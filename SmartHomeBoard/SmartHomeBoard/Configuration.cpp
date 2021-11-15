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


Unit* Configuration::FindUnit(uint16_t id) {
	if (units != NULL /*&& IsConfigReady*/) {
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
	Ethernet.begin(mac, ip);
}


void Configuration::Init() {
	Log = new Loger(200);
	ReadBoardId();
	EthClient = new EthernetClient();
	
	if (IsEthernetConnection) {
		InitializeServer();
    
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
}

Unit* Configuration::CreateTypedUnit(byte type) {

	Unit* u = NULL;
	if (type == UnitType::BUTTON) {
		u = new Button();
		if (u == NULL) {
			Log->Error(F1("Can't create Button Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::BUTTON;
	}
	else if (type == UnitType::RELAY) {

		u = new Relay();
		if (u == NULL) {
			Log->Error(F1("Can't create Relay Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::RELAY;
	}
	else if (type == UnitType::ONE_WIRE_BUS) {
		u = new OneWireBus();
		if (u == NULL) {
			Log->Error(F1("Can't create OneWire bus Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_BUS;
	}
	else if (type == UnitType::ONE_WIRE_THERMO) {
		u = new OneWireThermo();
		if (u == NULL) {
			Loger::Error();
			Log->Error(F1("Can't create OneWire Thermometer Unit"));
			Board::Reset(10000);
		}
		u->Type = UnitType::ONE_WIRE_THERMO;
	}
	else if (type == UnitType::POWER_METER) {
		u = new PowerMeter();
		if (u == NULL) {
			Loger::Error();
			Log->Error(F1("Can't create Power Meter Unit"));
			Board::Reset(10000);

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
}
