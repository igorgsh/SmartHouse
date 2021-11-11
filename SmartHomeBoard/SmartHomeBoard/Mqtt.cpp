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

extern Mqtt MqttClient;

void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	MqttClient.Callback(topic, (char*) payload, length);
}

Mqtt::Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, callbackFunc, EthClient) {
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;

}


bool Mqtt::MqttReconnect() {
	
	char topic[MQTT_TOPIC_LENGTH];
	bool res = false;

	if (Config.IsEthernetConnection) {
		if (!connected()) {
			Log.Debug(F("MqttReconnect"));
			if (connect(Config.BoardName)) {
				sprintf(topic, MQTT_CONFIG_RESPONSE, Config.BoardId);
				Subscribe(topic);
				sprintf(topic, MQTT_ACTIONS_RESPONSE, Config.BoardId);
				Subscribe(topic);
				//sprintf(topic, MQTT_RESET_BOARD, Config.BoardId);
				//Subscribe(topic);
				res = true;
			}
			else {
				Log.Error(F("MQTT connection Failed"));
				res = false;
			}
		}
	}
	return res;
}
uint16_t Mqtt::GetUnitId(const char* str, int offset) {
	return (str[offset] - '0') * 10 + str[offset + 1];

}

void Mqtt::Callback(const char* topic, const char* payLoad, unsigned int length) {

	if (length > 0) {
		createSafeStringFromCharPtr(strTopic, topic);
		createSafeStringFromCharPtr(strPayload, (char*)payLoad);
		char subscription[MQTT_TOPIC_LENGTH];

		Log.append(F("[")).append(strTopic).append(F("]:")).append(strPayload).append(F("#")).Debug();
		sprintf(subscription, MQTT_CONFIG_RESPONSE, Config.BoardId);
		if (strcmp(topic, subscription) == 0) {
			if (Config.isConfigRequested) {
				Config.UpdateConfig(payLoad);
			}
		}
		else {
			sprintf(subscription, MQTT_ACTIONS_RESPONSE, Config.BoardId);

			if (strcmp(topic, subscription) == 0) {
				if (Config.isActionRequested) {
					Config.UpdateActions(payLoad);
				}
			}
			else {
				int v;
				strPayload.toInt(v);

				if (strTopic.startsWith(MQTT_BUTTONS)) {
					Config.UpdateButton(GetUnitId(topic, strlen(MQTT_BUTTONS) + 2), v) ;
				}
				else {
					if (strTopic.startsWith(MQTT_RELAYS)) {
						Config.UpdateRelay(GetUnitId(topic, strlen(MQTT_RELAYS) + 2), v);					}
					else {
						if (strTopic.startsWith(MQTT_1WIREBUS)) {
							Config.UpdateOneWireBus(GetUnitId(topic, strlen(MQTT_1WIREBUS) + 2), v);
						}
						else {
							if (strTopic.startsWith(MQTT_1WIRETHERMO)) {
								Config.UpdateOneWireThermo(GetUnitId(topic, strlen(MQTT_1WIRETHERMO) + 2), v);
							}
							else {
								if (strTopic.startsWith(MQTT_POWERMETER)) {
									Config.UpdatePowerMeter(GetUnitId(topic, strlen(MQTT_POWERMETER) + 2), v);
								}
								else {
									if (strTopic.startsWith(MQTT_CONTACTOR)) {
										Config.UpdateContactor(GetUnitId(topic, strlen(MQTT_CONTACTOR) + 2), v);
									}
									else {
										if (strTopic.startsWith(MQTT_RESET_BOARD)) {
											Log.Info(F("Reset"));

											if (!strPayload.isEmpty() && strPayload[0] >= '0') {
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
	Log.Debug(F("Init MQTT"));
	long connectTry = 0;
	bool res = false;
	
	while (!res && connectTry <= MQTT_TRY_COUNT) {
		Log.append(F("Mqtt connect attempt=")).append(connectTry).Debug();
		res = MqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}
	
	if (!res) {
		Log.Error(F("Too many attempt of MQTT reconnect"));
	}
}
void Mqtt::MqttLoop() {

	static long lastConnected = 0;

	if (connected()) {
		bool res = loop();
		if (!res) {
			Log.Error(F("Failed loop"));
		}
		lastConnected = millis();
	}
	else {
		if (lastConnected + millis() <= MQTT_RETRY_TIME) {
			Log.Debug(F("Trying to reconnect MQTT"));
			bool res = MqttReconnect();
			if (res) {
				Config.Init();
			}
		}
	}
}

void Mqtt::PublishLog(DebugLevel level, const char* message) {
	if (connected()) {
		char topic[MQTT_TOPIC_LENGTH];
		sprintf(topic, MQTT_LOG, Config.BoardId, LOG_END[level]);
		publish(topic, message);
	}
}


void Mqtt::GetConfiguration() {

	uint8_t rnd = random(100, 999);
	char strRnd[3];
	char topic[MQTT_TOPIC_LENGTH];
	Config.IsConfigReady = false;
	sprintf(strRnd, "%3u", rnd);
	if (connected()) {
		sprintf(topic, MQTT_CONFIG_REQUEST, Config.BoardId);
		Publish(topic, strRnd);
		Config.isConfigRequested = true;
	}
	else {
		Config.isConfigRequested = false;
	}
}

void Mqtt::WatchDog() {

	//uint8_t rnd = random(0, 1000);
	char topic[MQTT_TOPIC_LENGTH];
	sprintf(topic, MQTT_WATCH_DOG, Config.BoardId);
	Publish(topic, String(Config.counter60).c_str());
}


void Mqtt::PublishUnit(const Unit* unit) {
	if (connected()) {
		char topic[MQTT_TOPIC_LENGTH];
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
				sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, unit->Type, unit->Id);
				sprintf(payload, "%u", unit->status);
				Publish(topic, payload);
			}
		}
	}
}

bool Mqtt::Publish(const char* topic, const char* payload) {
	if (connected()) {
		Log.append(F("Publish:[")).append(topic).append(F("]")).append(payload).Debug();
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
		bool isSubscriptionSuccess = true;
		Log.Debug(F("Subscribing Units..."));
		for (int i = 0; i < Config.numberUnits; i++) {
			SubscribeUnit(i);
		}
		delay(MQTT_RESUBSCRIPTION_DELAY);
		for (int i = 0; i < Config.numberUnits; i++) {

			MqttClient.MqttLoop();
			for (int j = 0; j < MQTT_RESUBSCRIBE_TRY_COUNT && !Config.units[i]->isSubscribed; j++) {
				SubscribeUnit(i);
				delay(MQTT_RESUBSCRIPTION_DELAY);
				MqttClient.MqttLoop();
			}
			if (!Config.units[i]->isSubscribed) {
				isSubscriptionSuccess = false;
			}
		}
		if (!isSubscriptionSuccess) {
			Log.Error(F("Some units are not subscribed"));
		}
		Log.Debug(F("End subscription"));
	}
}

void Mqtt::Subscribe(const char* topic) {
	Log.append(F("Subscribe [")).append(topic).append(F("]")).Debug();
	if (connected()) {
		subscribe(topic);
	}
}
