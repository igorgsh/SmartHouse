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
/**/
void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	MqttClient.Callback(topic, payload, length);
}

Mqtt::Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, callbackFunc, EthClient) {
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;

}


bool Mqtt::MqttReconnect() {
	
	char topic[TOPIC_LENGTH];
	bool res = false;

	if (Config.IsEthernetConnection) {
		if (!connected()) {
			Loger::Debug("MqttReconnect");
			if (connect(Config.BoardName.c_str())) {
				sprintf(topic, MQTT_CONFIG_RESPONSE, Config.BoardId);
				Subscribe(topic);
				sprintf(topic, MQTT_ACTIONS_RESPONSE, Config.BoardId);
				Subscribe(topic);
				//sprintf(topic, MQTT_RESET_BOARD, Config.BoardId);
				//Subscribe(topic);
				res = true;
			}
			else {
				Loger::Error("Failed, rc=" + String(MqttClient.state(), DEC));
				res = false;
			}
		}
	}
	return res;
}

void Mqtt::Callback(char* topic, uint8_t* payLoad, unsigned int length) {

	if (length > 0) {
		String strTopic = String(topic);
		String strPayload = String((char*)payLoad).substring(0, length);
		char subscription[TOPIC_LENGTH];

		Loger::Debug("[" + strTopic + "]:" + strPayload + "#");
		sprintf(subscription, MQTT_CONFIG_RESPONSE, Config.BoardId);
		if (strcmp(topic, subscription) == 0) {
			if (Config.isConfigRequested) {
				Config.UpdateConfig(strPayload);
			}
		}
		else {
			sprintf(subscription, MQTT_ACTIONS_RESPONSE, Config.BoardId);

			if (strcmp(topic, subscription) == 0) {
				if (Config.isActionRequested) {
					Config.UpdateActions(strPayload);
				}
			}
			else {
				if (strTopic.startsWith(MQTT_BUTTONS)) {
					Loger::Debug("Update Button");

					Config.UpdateButton(strTopic.substring(strlen(MQTT_BUTTONS) + 2), strPayload);
				}
				else {
					if (strTopic.startsWith(MQTT_RELAYS)) {
						//Loger::Debug("Update Relay");

						Config.UpdateRelay(strTopic.substring(strlen(MQTT_RELAYS) + 2), strPayload);
					}
					else {
						if (strTopic.startsWith(MQTT_1WIREBUS)) {
							//Loger::Debug("Update 1-wire bus");

							Config.UpdateOneWireBus(strTopic.substring(strlen(MQTT_1WIREBUS) + 2), strPayload);
						}
						else {
							if (strTopic.startsWith(MQTT_1WIRETHERMO)) {
								//Loger::Debug("Update 1-wire thermo");

								Config.UpdateOneWireThermo(strTopic.substring(strlen(MQTT_1WIRETHERMO) + 2), strPayload);
							}
							else {
								if (strTopic.startsWith(MQTT_POWERMETER)) {
									//Loger::Debug("Update Powermeter");
									Config.UpdatePowerMeter(strTopic.substring(strlen(MQTT_POWERMETER) + 2), strPayload);
								}
								else {
									if (strTopic.startsWith(MQTT_CONTACTOR)) {
										//Loger::Debug("Update Contactor");
										Config.UpdateContactor(strTopic.substring(strlen(MQTT_CONTACTOR) + 2), strPayload);
									}
									else {
										if (strTopic.startsWith(MQTT_RESET_BOARD)) {
											sprintf(subscription, MQTT_RESET_BOARD, Config.BoardId);
											Loger::Debug("Reset");

											if (strTopic.startsWith((String)subscription) && strPayload != NULL && strPayload[0] >= '0') {
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
	Loger::Debug("Init MQTT");
	long connectTry = 0;
	bool res = false;
	
	while (!res && connectTry <= MQTT_TRY_COUNT) {
		Loger::Debug("Mqtt connect attempt=" + String(connectTry));
		res = MqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}
	
	if (!res) {
		Loger::Debug("Too many attempt of MQTT reconnect");
	}
}
void Mqtt::MqttLoop() {

	static long lastConnected = 0;

	if (connected()) {
		bool res = loop();
		if (!res) {
			Loger::Error("Failed loop");
		}
		lastConnected = millis();
	}
	else {
		if (lastConnected + millis() <= MQTT_RETRY_TIME) {
			Loger::Debug("Trying to reconnect MQTT");
			bool res = MqttReconnect();
			if (res) {
				Config.Init();
			}
		}
	}

}

void Mqtt::PublishLog(DebugLevel level, String message) {
	if (connected()) {
		char topic[TOPIC_LENGTH];
		sprintf(topic, MQTT_LOG, Config.BoardId, LOG_END[level]);
		publish(topic, message.c_str());
	}
}


void Mqtt::GetConfiguration() {

	uint8_t rnd = random(0, 1000);
	char topic[TOPIC_LENGTH];
	Config.IsConfigReady = false;
	if (connected()) {
		sprintf(topic, MQTT_CONFIG_REQUEST, Config.BoardId);
		Publish(topic, String(rnd).c_str());
		Config.isConfigRequested = true;
	}
	else {
		Config.isConfigRequested = false;
	}
}

void Mqtt::WatchDog() {

	//uint8_t rnd = random(0, 1000);
	char topic[TOPIC_LENGTH];
	sprintf(topic, MQTT_WATCH_DOG, Config.BoardId);
	Publish(topic, String(Config.counter60).c_str());
}


void Mqtt::PublishUnit(const Unit *unit) {
	if (connected()) {
		//if (unit->isSubscribed) {
			char topic[TOPIC_LENGTH];
			char payload[PAYLOAD_LENGTH];
			const char *unitPrefix;
			byte unitType = 0;
			if (unit->Type == ::POWER_METER) {
				PowerMeter *p = (PowerMeter*)unit;
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
//		}
//		else {
//			Loger::Debug("Publish Unit:" + String(unit->Id, DEC) + " is not subscribed!");
//		}
	}
}

bool Mqtt::Publish(const char* topic, const char* payload) {
	if (connected()) {
		Loger::Debug("Publish:[" + String(topic) + "]" + String(payload));
		return publish(topic, payload);
	}
	else {
		return false;
	}
}

void Mqtt::GetActions() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	char topic[TOPIC_LENGTH];
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
		char topic[TOPIC_LENGTH];
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
		Loger::Debug("Subscribing Units...");
		for (int i = 0; i < Config.numberUnits; i++) {
			//Loger::Debug("Subscribe:" + String(i));
			SubscribeUnit(i);
			//MqttClient.MqttLoop();
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
			Loger::Error("Some units are not subscribed");
		}
		Loger::Debug("End subscription");
	}
}

void Mqtt::Subscribe(const char* topic) {
	Loger::Debug("Subscribe [" + String(topic) + "]");
	if (connected()) {
		subscribe(topic);
	}
}
