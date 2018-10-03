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
	MqttClient.Callback(topic, payload, length);
}

Mqtt::Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, callbackFunc, EthClient) {
	mqttWaiting = MQTT_RECONNECT_TIME;

}


bool Mqtt::MqttReconnect() {
	
	//Если соединение MQTT неактивно, то пытаемся установить его и опубликовать/подписаться
	static unsigned long errorTime = 0;
	char topic[TOPIC_LENGTH];

	if (!connected()) {
		if (!firstConnect) {
			Loger::Error("MQTT Failed:" + String(errorTime));
		}
		if (errorTime == 0 || errorTime + mqttWaiting < millis()) { //reconnect
			Loger::Debug("MqttReconnect:" + String(errorTime) + ":" + String(errorTime + mqttWaiting) + ":" + String(millis()));

			//Подключаемся и подписываемся
			if (connect(Config.BoardName.c_str())) {
				mqttWaiting = MQTT_RECONNECT_TIME;
				errorTime = 0;

				//if (firstConnect) {
					//Подписываемся на все темы
					//MqttClient.subscribe(TOPIC_CONFIG_LENGTH);
					sprintf(topic, MQTT_CONFIG_RESPONSE, Config.BoardId);
					Subscribe(topic);
					sprintf(topic, MQTT_ACTIONS_RESPONSE, Config.BoardId);
					Subscribe(topic);
					//sprintf(topic, MQTT_RESET_BOARD, Config.BoardId);
					//Subscribe(topic);
					firstConnect = false;
					if (Config.IsConfigReady) {
						SubscribeUnits();
					}
				//}
				loop();
			}
			else {
				mqttWaiting *= 2;
				Loger::Error("Failed, rc=" + String(MqttClient.state(), DEC));
				errorTime = millis();
			}
		}
	}
	return (errorTime == 0);
}

void Mqtt::Callback(char* topic, uint8_t* payLoad, unsigned int length) {
	//преобразуем тему(topic) и значение (payload) в строку
	//Debug2("Point6.4.2:", memoryFree());

	//Loger::Debug("Topic:" + String(topic) + ";payload length=" + String(length, DEC));
	if (length > 0) {
		String strTopic = String(topic);
		String strPayload = String((char*)payLoad).substring(0, length);
		char subscription[TOPIC_LENGTH];
		//Исследуем что "прилетело" от сервера по подписке:
		Loger::Debug("[" + strTopic + "]:" + strPayload + "#");
		sprintf(subscription, MQTT_CONFIG_RESPONSE, Config.BoardId);
		//Loger::Debug("subscription=" + String(subscription));
		//Loger::Debug("strcmp=" + String(strcmp(topic, subscription)));
		if (strcmp(topic, subscription) == 0) {
			//Loger::Debug("Update Config");
			Config.UpdateConfig(strPayload);
			//Loger::Debug("Point 1");
		}
		else {
			sprintf(subscription, MQTT_ACTIONS_RESPONSE, Config.BoardId);

			if (strcmp(topic, subscription) == 0) {
				//Loger::Debug("Update Actions");
				Config.UpdateActions(strPayload);
			}
			else {
				if (strTopic.startsWith(MQTT_BUTTONS)) {
					//Loger::Debug("Update Button");

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
									//Nothing to do
									Config.UpdatePowerMeter(strTopic.substring(strlen(MQTT_POWERMETER) + 2), strPayload);
								}
								else if (strTopic.startsWith(MQTT_RESET_BOARD)) {
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

void Mqtt::InitMqtt(void) {
//	SetTopicNames();
	Loger::Debug("Init MQTT");
	MqttReconnect();
}
void Mqtt::MqttLoop() {
	//Loger::Debug("Start Mqttloop");
	if (Config.IsEthernetConnection) {
		if (MqttReconnect()) {
			
			bool res = loop();
			if (!res) {
				Loger::Error("Failed loop");
			}
		}
		else {
			Loger::Error("Can't reconnect MQTT");
		}
	}
	//Loger::Debug("End Mqttloop");
}

void Mqtt::PublishLog(DebugLevel level, String message) {
	char topic[TOPIC_LENGTH];
	sprintf(topic, MQTT_LOG,Config.BoardId,  LOG_END[level]);
	publish(topic, message.c_str());
}


void Mqtt::GetConfiguration() {

	uint8_t rnd = random(0, 1000);
	char topic[TOPIC_LENGTH];
	Config.IsConfigReady = false;
	sprintf(topic, MQTT_CONFIG_REQUEST, Config.BoardId);
	Config.isConfigRequested = true;
	Publish(topic, String(rnd).c_str());
}

void Mqtt::WatchDog() {

	//uint8_t rnd = random(0, 1000);
	char topic[TOPIC_LENGTH];
	sprintf(topic, MQTT_WATCH_DOG, Config.BoardId);
	Publish(topic, String(Config.counter60).c_str());
}


void Mqtt::PublishUnit(const Unit *unit) {
	if (unit->isSubscribed) {
		if (Config.IsEthernetConnection) {
			char topic[TOPIC_LENGTH];
			char payload[PAYLOAD_LENGTH];
			const char *unitPrefix;
			if (UnitType::POWER_METER) {
				PowerMeter *p = (PowerMeter*)unit;
				p->PublishAll();
				/*
				char topic0[TOPIC_LENGTH];
				sprintf(topic0, "%s%s%c%04d", MQTT_POWERMETER, MQTT_SEPARATOR, unit->Type, unit->Id);
				PowerMeter *p = (PowerMeter*)unit;
				sprintf(topic, "%s%sVoltage", topic0, MQTT_SEPARATOR);
				sprintf(payload, "%02f", p->voltage());
				Publish(topic, payload);
				sprintf(topic, "%s%sCurrent", topic0, MQTT_SEPARATOR);
				sprintf(payload, "%02f", p->current());
				Publish(topic, payload);
				sprintf(topic, "%s%sPower", topic0, MQTT_SEPARATOR);
				sprintf(payload, "%02f", p->power());
				Publish(topic, payload);
				sprintf(topic, "%s%sEnergy", topic0, MQTT_SEPARATOR);
				sprintf(payload, "%02f", p->energy());
				Publish(topic, payload);
				*/
			}
			else {
				switch (unit->Type) {
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
				}
				sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, unit->Type, unit->Id);
				sprintf(payload, "%u", unit->status);
				Publish(topic, payload);

			}
		}
	}
	else {
		Loger::Debug("Publish Unit:" + String(unit->Id, DEC) + " is not subscribed!");
	}
}

bool Mqtt::Publish(const char* topic, const char* payload) {

	Loger::Debug("Publish:[" + String(topic) + "]" + String(payload));
	return publish(topic, payload);
}

void Mqtt::GetActions() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	char topic[TOPIC_LENGTH];
	//Debug2("Action:", buf);
	Config.IsConfigReady = false;
	Config.IsActionsReady = false;

	sprintf(topic, MQTT_ACTIONS_REQUEST, Config.BoardId);
	Config.isActionRequested = true;
	Publish(topic, buf);
}

void Mqtt::SubscribeUnit(int unitNumber) {
	char topic[TOPIC_LENGTH];
	const char* unitPrefix=NULL;
	//Loger::Debug("Point 1");
	//Loger::Debug("UnitType["+String(unitNumber,DEC) +"]=" + String(Config.units[unitNumber]->Type,DEC));
	//Loger::Debug("Point 2");
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
		}
		if (unitPrefix != NULL) {
			sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, Config.units[unitNumber]->Type, Config.units[unitNumber]->Id);
			Subscribe(topic);
		}
	}
}


void Mqtt::SubscribeUnits() {
	bool isSubscriptionSuccess = true;
	Loger::Debug("Subscribing Units...");
	for (int i = 0; i < Config.numberUnits; i++) {
		//Loger::Debug("i=" + String(i, DEC));
		SubscribeUnit(i);
//		delay(MQTT_RESUBSCRIPTION_DELAY);
		MqttClient.MqttLoop();
	}
	delay(MQTT_RESUBSCRIPTION_DELAY);
	for (int i = 0; i < Config.numberUnits; i++) {
		//Loger::Debug("Point 1");
		MqttClient.MqttLoop();
		for (int j = 0; j < MQTT_RESUBSCRIBE_TRY_COUNT && !Config.units[i]->isSubscribed; j++) {
			//Loger::Debug("Point 2");
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

void Mqtt::Subscribe(const char* topic) {
	Loger::Debug("Subscribe [" + String(topic) + "]");
	if (Config.IsEthernetConnection) {
		subscribe(topic);
	}
}
