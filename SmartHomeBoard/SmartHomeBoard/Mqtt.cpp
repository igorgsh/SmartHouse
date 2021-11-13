// 
// 
// 
/*
Name:		Mqtt
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/

#include "Mqtt.h"
#include "Loger.h"
#include "definitions.h"
#include "utils.h"
#include "Board.h"
#include "PowerMeter.h"
#include "Board.h"
#include "Configuration.h"

extern Configuration Config;


void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	//Log.append("Point1:").append("topic=").append(topic).append(" length=").append((int)strlen(topic)).append(" payload=").append((int)strlen((char*)payload)).Debug();
	//Log.append("Point3:").append("topic=").append(topic).append(" length=").append((int)strlen(topic)).append(" payload=").append((int)strlen((char*)payload)).Debug();
	char* pl = (char*)payload;
	pl[length] = 0;

	
	//Log.append("Topic=").append(topic).Debug();
	//Log.append("PayLoad=").append(pl).Debug();
	Config.MqttClient->Callback(topic, pl, length);
}

Mqtt::Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, callbackFunc, *(Config.EthClient)) {
	Serial.println("MQTT!");
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;
}


bool Mqtt::MqttReconnect() {
	
	bool res = false;

	if (Config.IsEthernetConnection) {
		if (!connected()) {
			Config.Log->Debug(F1("MqttReconnect"));
			if (connect(Config.BoardName)) {
				sprintf(topicBuff, MQTT_CONFIG_RESPONSE, Config.BoardId);
				Subscribe(topicBuff);
				sprintf(topicBuff, MQTT_ACTIONS_RESPONSE, Config.BoardId);
				Subscribe(topicBuff);
				//sprintF1(topic, MQTT_RESET_BOARD, Config.BoardId);
				//Subscribe(topic);
				res = true;
			}
			else {
				Config.Log->Error(F1("MQTT connection Failed"));
				res = false;
			}
		}
	}
	return res;
}
uint16_t Mqtt::GetUnitId(const char* str, int offset) {
	return atoi(str + offset);
}

void Mqtt::Callback(const char* topic, const char* payLoad, unsigned int length) {

	if (length > 0) {
		
		Config.Log->append(F1("[")).append(topic).append(F1("]:")).append(payLoad).append(F1("#")).Debug();
		sprintf(topicBuff, MQTT_CONFIG_RESPONSE, Config.BoardId);
		if (strcmp(topic, topicBuff) == 0) {
			if (Config.isConfigRequested) {
				Config.UpdateConfig(payLoad);
			}
		}
		else {
			sprintf(topicBuff, MQTT_ACTIONS_RESPONSE, Config.BoardId);

			if (strcmp(topic, topicBuff) == 0) {
				if (Config.isActionRequested) {
					Config.UpdateActions(payLoad);
				}
			}
			else {
				int v = atoi(payLoad);
				

				if (strncmp(topic,MQTT_BUTTONS,strlen(MQTT_BUTTONS))) {
					Config.UpdateButton(GetUnitId(topic, strlen(MQTT_BUTTONS) + 2), v) ;
				}
				else {
					if (strncmp(topic,MQTT_RELAYS,strlen(MQTT_RELAYS))) {
						Config.UpdateRelay(GetUnitId(topic, strlen(MQTT_RELAYS) + 2), v);					}
					else {
						if (strncmp(topic,MQTT_1WIREBUS,strlen(MQTT_1WIREBUS))) {
							Config.UpdateOneWireBus(GetUnitId(topic, strlen(MQTT_1WIREBUS) + 2), v);
						}
						else {
							if (strncmp(topic,MQTT_1WIRETHERMO,strlen(MQTT_1WIRETHERMO))) {
								Config.UpdateOneWireThermo(GetUnitId(topic, strlen(MQTT_1WIRETHERMO) + 2), v);
							}
							else {
								if (strncmp(topic,MQTT_POWERMETER, strlen(MQTT_POWERMETER))) {
									Config.UpdatePowerMeter(GetUnitId(topic, strlen(MQTT_POWERMETER) + 2), v);
								}
								else {
									if (strncmp(topic,MQTT_CONTACTOR, strlen(MQTT_CONTACTOR))) {
										Config.UpdateContactor(GetUnitId(topic, strlen(MQTT_CONTACTOR) + 2), v);
									}
									else {
										if (strncmp(topic, MQTT_RESET_BOARD, strlen(MQTT_RESET_BOARD))) {
											Config.Log->Info(F1("Reset"));

											if (v > 0) {
												Board::Reset(10000);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}



void Mqtt::InitMqtt(void) {
	long connectTry = 0;
	bool res = false;
	for (int i = 0; i < 7; i++) {
		sprintf(topicLog[i], MQTT_LOG, Config.BoardId, LOG_END[i]);
	}

	while (!res && connectTry <= MQTT_TRY_COUNT) {
		res = MqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}
	
	if (!res) {
		Config.Log->Error(F1("Too many attempt of MQTT reconnect"));
	}
}
void Mqtt::MqttLoop() {

	static long lastConnected = 0;

	if (connected()) {
		bool res = loop();
		if (!res) {
			Config.Log->Error(F1("Failed loop"));
		}
		lastConnected = millis();
	}
	else {
		if (lastConnected + millis() <= MQTT_RETRY_TIME) {
			Config.Log->Debug(F1("Trying to reconnect MQTT"));
			bool res = MqttReconnect();
			if (res) {
				Config.Init();
			}
		}
	}
}

void Mqtt::PublishLog(DebugLevel level, const char* message) {

	if (connected()) {
		publish(topicLog[level], message);
	}
}


void Mqtt::GetConfiguration() {

	uint8_t rnd = random(100, 999);
	char strRnd[3];
	Config.IsConfigReady = false;
	sprintf(strRnd, "%3u", rnd);
	if (connected()) {
		sprintf(topicBuff, MQTT_CONFIG_REQUEST, Config.BoardId);
		Publish(topicBuff, strRnd);
		Config.isConfigRequested = true;
	}
	else {
		Config.isConfigRequested = false;
	}
}

void Mqtt::WatchDog() {

	//uint8_t rnd = random(0, 1000);
	sprintf(topicBuff, MQTT_WATCH_DOG, Config.BoardId);
	Publish(topicBuff, String(Config.counter60).c_str());
}


void Mqtt::PublishUnit(const Unit* unit) {
	if (connected()) {
		char payload[MQTT_PAYLOAD_LENGTH];
		const char* unitPrefix;
		byte unitType = 0;
		if (unit->Type == ::POWER_METER) {
			PowerMeter* p = (PowerMeter*)unit;
			p->PublishAll();
		}
		else {
			switch (unit->Type) {
			case UnitType::BUTTON: {
				unitPrefix = MQTT_BUTTONS;
				unitType = unit->Type;
				break;
			}
			case UnitType::RELAY: {
				unitPrefix = MQTT_RELAYS;
				unitType = unit->Type;
				break;
			}
			case UnitType::ONE_WIRE_BUS: {
				unitPrefix = MQTT_1WIREBUS;
				unitType = unit->Type;
				break;
			}
			case UnitType::ONE_WIRE_THERMO: {
				unitPrefix = MQTT_1WIRETHERMO;
				unitType = unit->Type;
				break;
			}
			case UnitType::VIRTUAL_BUTTON: {
				unitPrefix = MQTT_VIRTUAL_BUTTONS;
				unitType = UnitType::BUTTON;
				break;
			}
			case UnitType::CONTACTOR: {
				unitPrefix = MQTT_CONTACTOR;
				unitType = UnitType::CONTACTOR;
				break;
			}
			}
			if (unitType != 0) {
				sprintf(topicBuff, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, unit->Type, unit->Id);
				sprintf(payload, "%u", unit->status);
				Publish(topicBuff, payload);
			}
		}
	}
}

bool Mqtt::Publish(const char* topic, const char* payload) {
	Config.Log->append(F1("Publish [")).append(topic).append(F1("]:")).append(payload).Debug();
	if (connected()) {
		return publish(topic, payload);
	}
	else {
		return false;
	}
}

void Mqtt::GetActions() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	char topic[MQTT_TOPIC_LENGTH];
	Config.IsConfigReady = false;
	Config.IsActionsReady = false;
	if (connected()) {
		sprintf(topic, MQTT_ACTIONS_REQUEST, Config.BoardId);
		Config.isActionRequested = true;
		Publish(topic, buf);
	}
	else {
		Config.isActionRequested = true;
	}
}

void Mqtt::SubscribeUnit(int unitNumber) {
	if (connected()) {
		char topic[MQTT_TOPIC_LENGTH];
		const char* unitPrefix = NULL;
		if (Config.units[unitNumber]->Type == UnitType::POWER_METER) {
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_VOLTAGE);
			Subscribe(topic);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_CURRENT);
			Subscribe(topic);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_POWER);
			Subscribe(topic);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_ENERGY);
			Subscribe(topic);
		}
		else {
			switch (Config.units[unitNumber]->Type) {
			case UnitType::BUTTON: {
				unitPrefix = MQTT_BUTTONS;
				break;
			}
			case UnitType::RELAY: {
				unitPrefix = MQTT_RELAYS;
				break;
			}
			case UnitType::ONE_WIRE_BUS: {
				unitPrefix = MQTT_1WIREBUS;
				break;
			}
			case UnitType::ONE_WIRE_THERMO: {
				unitPrefix = MQTT_1WIRETHERMO;
				break;
			}
			case UnitType::CONTACTOR: {
				unitPrefix = MQTT_CONTACTOR;
				break;
			}
			}
			if (unitPrefix != NULL) {
				sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, Config.units[unitNumber]->Type, Config.units[unitNumber]->Id);
				Subscribe(topic);
			}
		}
	}
}


void Mqtt::SubscribeUnits() {
	if (connected()) {
//		bool isSubscriptionSuccess = true;
		Config.Log->Debug(F1("Subscribing Units..."));
		for (int i = 0; i < Config.numberUnits; i++) {
			SubscribeUnit(i);
		}
		/*
		delay(MQTT_RESUBSCRIPTION_DELAY);
		for (int i = 0; i < Config.numberUnits; i++) {

			MqttClient.MqttLoop();
			for (int j = 0; j < MQTT_RESUBSCRIBE_TRY_COUNT && !Config.units[i]->isSubscribed; j++) {
				SubscribeUnit(i);
				delay(MQTT_RESUBSCRIPTION_DELAY);
				MqttClient.MqttLoop();
			}
			if (!Config.units[i]->isSubscribed) {
				Log.append("Subscription Failed. Unit id=").append(Config.units[i]->Id).Debug();
				isSubscriptionSuccess = false;
			}
		}
		if (!isSubscriptionSuccess) {
			Log.Error(F1("Some units are not subscribed"));
		}
		*/
		Config.Log->append(F1("End subscription:")).append(Config.numberUnits).Debug();
	}
}

void Mqtt::Subscribe(const char* topic) {
	if (connected()) {
		subscribe(topic);
	}
}
