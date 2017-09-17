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
			if (connect(topicPrefix)) {
				mqttWaiting = MQTT_RECONNECT_TIME;
				errorTime = 0;

				if (firstConnect) {
					//Подписываемся на все темы
					//MqttClient.subscribe(TOPIC_CONFIG_LENGTH);
					sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_CONFIG_RESPONSE);
					Loger::Debug("Subscribe:" + String(topic));
					subscribe(topic);
					sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_ACTIONS_RESPONSE);
					Loger::Debug("Subscribe:" + String(topic));
					subscribe(topic);
					sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_RESET_BOARD);
					Loger::Debug("Subscribe:" + String(topic));
					subscribe(topic);
					firstConnect = false;
				}
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

void Mqtt::Callback(char* topic, uint8_t* payload, unsigned int length) {
	//преобразуем тему(topic) и значение (payload) в строку
	//Debug2("Point6.4.2:", memoryFree());

	payload[length] = '\0';
	String strTopic = String(topic);
	String strPayload = String((char*)payload);
	//Исследуем что "прилетело" от сервера по подписке:
	Loger::Debug("[" + strTopic + "]:" + strPayload);
	//Debug2("Point6.4.4:", memoryFree());
	if (strTopic.startsWith((String)(SUFFIX_CONFIG_RESPONSE), strlen(topicPrefix) + 1)) {
		Config.UpdateConfig(strPayload.c_str());
	}
	else if (strTopic.startsWith((String)(SUFFIX_ACTIONS_RESPONSE), strlen(topicPrefix) + 1)) {
		Config.UpdateActions(strPayload.c_str());
	}
	else if (strTopic.startsWith((String)(SUFFIX_GET_BUTTONS), strlen(topicPrefix) + 1)) {
		Config.UpdateButton(strTopic.substring(strlen(topicPrefix) + 1 + strlen(SUFFIX_GET_BUTTONS) + 2), strPayload);
	}
	else if (strTopic.startsWith((String)(SUFFIX_GET_RELAYS), strlen(topicPrefix) + 1)) {
		Config.UpdateRelay(strTopic.substring(strlen(topicPrefix) + 1 + strlen(SUFFIX_GET_RELAYS) + 2), strPayload);
	}
	else if (strTopic.startsWith((String)(SUFFIX_GET_1WIREBUS), strlen(topicPrefix) + 1)) {
		Config.UpdateOneWireBus(strTopic.substring(strlen(topicPrefix) + 1 + strlen(SUFFIX_GET_1WIREBUS) + 2), strPayload);
	}
	else if (strTopic.startsWith((String)(SUFFIX_GET_1WIRETHERMO), strlen(topicPrefix) + 1)) {
		Config.UpdateOneWireThermo(strTopic.substring(strlen(topicPrefix) + 1 + strlen(SUFFIX_GET_1WIRETHERMO) + 2), strPayload);
	}
	else if (strTopic.startsWith((String)(SUFFIX_RESET_BOARD), strlen(topicPrefix) + 1)) {
		Board::Reset(10000);
	}
	//Debug2("Point6.4.6:", memoryFree());
}

void Mqtt::SetTopicNames() {
	int ind = strlen(topicPrefix) - 2;
	topicPrefix[ind] = (int)'0' + (int)((Config.BoardId >> 4) & 0x0F);
	topicPrefix[ind + 1] = (int)'0' + (int)(Config.BoardId & 0x0F);
}

void Mqtt::InitMqtt(void) {
	//Debug2("Point5.2:", memoryFree());
	SetTopicNames();
	//Debug2("Point5.4:", memoryFree());
	Loger::Debug("Init MQTT");
	MqttReconnect();
	//Debug2("Point5.6:", memoryFree());
}
void Mqtt::MqttLoop() {
	if (Config.IsEthernetConnection) {
		if (MqttReconnect()) {
			
			//Loger::Debug("Loop!");
			bool res = loop();
			if (!res) {
				Loger::Error("False loop");
			}

		}
	}
}

void Mqtt::PublishLog(DebugLevel level, String message) {
	char topic[TOPIC_LENGTH];
	sprintf(topic, "%s%s%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_LOG, MQTT_SEPARATOR, LOG_END[level]);
	publish(topic, message.c_str());
}


void Mqtt::GetConfiguration() {

	uint8_t rnd = random(0, 1000);
	char topic[TOPIC_LENGTH];
	Config.IsConfigReady = false;
	strcpy(topic, topicPrefix);
	strcat(topic, MQTT_SEPARATOR);
	strcat(topic, SUFFIX_CONFIG_REQUEST);
	Loger::Debug("Request:" + String(topic));
	publish(topic, String(rnd).c_str());
}

void Mqtt::PublishUnit(const Unit *unit) {
	if (Config.IsEthernetConnection) {
		char topic[TOPIC_LENGTH];
		char middle[TOPIC_LENGTH];
		switch (unit->Type) {
		case UnitType::BUTTON: {
			strcpy(middle, SUFFIX_PUT_BUTTONS);
			break;
		}
		case UnitType::RELAY: {
			strcpy(middle, SUFFIX_PUT_RELAYS);
			break;
		}
		case UnitType::ONE_WIRE_BUS: {
			strcpy(middle, SUFFIX_PUT_1WIREBUS);
			break;
		}
		case UnitType::ONE_WIRE_THERMO: {
			strcpy(middle, SUFFIX_PUT_1WIRETHERMO);
			break;
		}
		}
		sprintf(topic, "%s%s%s%s%c%04d", topicPrefix, MQTT_SEPARATOR, middle, MQTT_SEPARATOR, unit->Type, unit->Id);
		char payload[PAYLOAD_LENGTH];
		sprintf(payload, "%u", unit->status);
		Loger::Debug("Publish:[" + String(topic) + "]:" + String(payload));
		MqttClient.publish(topic, payload);
	}
}

void Mqtt::GetActions() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	char topic[TOPIC_LENGTH];
	//Debug2("Action:", buf);
	Config.IsConfigReady = false;
	Config.IsActionsReady = false;

	sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_ACTIONS_REQUEST);
	Loger::Debug("Request:" + String(topic));

	publish(topic, buf);
}

void Mqtt::SubscribeUnits() {
	char topic[TOPIC_LENGTH];
	Loger::Debug("Subscribing Units...");
	for (int i = 0; i < Config.numberUnits; i++) {

		switch (Config.units[i]->Type) {
		case UnitType::BUTTON: {
			sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_GET_BUTTONS);
			break;
		}
		case UnitType::RELAY: {
			sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_GET_RELAYS);
			break;
		}
		case UnitType::ONE_WIRE_BUS: {
			sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_GET_1WIREBUS);
			break;
		}
		case UnitType::ONE_WIRE_THERMO: {
			sprintf(topic, "%s%s%s", topicPrefix, MQTT_SEPARATOR, SUFFIX_GET_1WIRETHERMO);
			break;
		}
		}
		sprintf(topic, "%s%s%c%04d", topic, MQTT_SEPARATOR, Config.units[i]->Type, Config.units[i]->Id);
		Subscribe(topic);
		Loger::Debug("Subscribe:" + String(topic));
	}
}

void Mqtt::Subscribe(char* topic) {
	if (Config.IsEthernetConnection) {
		subscribe(topic);
	}
}
