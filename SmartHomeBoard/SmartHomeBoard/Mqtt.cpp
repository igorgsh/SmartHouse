// 
// 
// 
/*
Name:		Mqtt
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/
#include "mqtt.h"
#include "Loger.h"
#include "definitions.h"

extern Mqtt MqttClient;

void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	MqttClient.Callback(topic, payload, length);
}


/*
String GetDeviceId(String topic) {
	int index = topic.lastIndexOf("/");

	if (index > 0) {
		return topic.substring(index + 1);
	}
	else {
		return "";
	}
}

int ToInt(String s) {
	if (s.equals("null"))
		return 0;
	else if (s.equals("true"))
		return 1;
	else if (s.equals("false"))
		return 0;
	return s.toInt();
}
*/

bool Mqtt::MqttReconnect() {
	//Если соединение MQTT неактивно, то пытаемся установить его и опубликовать/подписаться
	static unsigned long errorTime = 0;
	char topic[TOPIC_LENGTH];

	if (!connected()) {
		if (errorTime == 0 || errorTime + MQTT_RECONNECT_TIME < millis()) { //reconnect

			SerialLog(D_WARN, "Connect to MQTT-broker...  ");
			//Подключаемся и подписываемся
			if (connect(topicPrefix)) {
				//Log(D_INFO, "MQTT connected");
				errorTime = 0;
				//Подписываемся на все темы
				//MqttClient.subscribe(TOPIC_CONFIG_LENGTH);
				sprintf(topic, "%s/%s", topicPrefix, SUFFIX_CONFIG_RESPONSE);
				Debug2("Subscribe:", topic);
				subscribe(topic);
				sprintf(topic, "%s/%s", topicPrefix, SUFFIX_ACTIONS_RESPONSE);
				Debug2("Subscribe:", topic);
				subscribe(topic);
				//SubscribeButtons();
				//Debug("Buttons subscribed");
				//SubscribeLigts();
				//Debug("Lights subscribed");
				//SubscribeRelays();
				//Debug("Relays subscribed");
				//Receive all messages
				loop();
			}
			else {
				//Если не подключились, ждем 10 секунд и пытаемся снова
				SerialLog(D_ERROR, "Failed, rc=");
				SerialLog2_(D_ERROR, MqttClient.state(), DEC);
				SerialLog(D_ERROR, " try again in 10 seconds");
				errorTime = millis();
			}
		}
	}

	return (errorTime == 0);
}
void Mqtt::Callback(char* topic, uint8_t* payload, unsigned int length) {
	//преобразуем тему(topic) и значение (payload) в строку
	payload[length] = '\0';
	String strTopic = String(topic);
	String strPayload = String((char*)payload);
	//Исследуем что "прилетело" от сервера по подписке:
	Debug_("[");
	Debug_(strTopic);
	Debug_("]:");
	Debug(strPayload);
	if (strTopic.startsWith((String)(SUFFIX_CONFIG_RESPONSE), strlen(topicPrefix) + 1)) {
		Config.UpdateConfig(strPayload.c_str());
	}
	else if (strTopic.startsWith((String)(SUFFIX_ACTIONS_RESPONSE), strlen(topicPrefix) + 1)) {
		Config.UpdateActions(strPayload.c_str());
	}
	/*
	if (strTopic.startsWith(TOPIC_SUBSCRIPTION_BUTTONS)) {
		//MessageButton(strTopic, strPayload);
		//Debug("Point 1.1")
	}
	else if (strTopic.startsWith(TOPIC_SUBSCRIPTION_LIGHTS)) {
		//MessageLight(strTopic, strPayload);
		//Debug("Point 1.2")
	}
	else if (strTopic.startsWith(TOPIC_SUBSCRIPTION_RELAYS)) {
		//MessageRelay(strTopic, strPayload);
		//Debug("Point 1.3")
	}
	//Debug("Point 2");
	*/
}

void Mqtt::SetTopicNames() {
	char buf[3];
	int ind = strlen(topicPrefix) - 2;
	sprintf(buf, "%02X", Config.BoardId);
	topicPrefix[ind] = buf[0];
	topicPrefix[ind + 1] = buf[1];
}

void Mqtt::InitMqtt(void) {
	SetTopicNames();
	MqttReconnect();
}

void Mqtt::MqttLoop() {
	if (MqttReconnect()) {
		loop();
	}
}
void Mqtt::PublishLog(DebugLevel level, String message) {
	char topic[TOPIC_LENGTH];
	sprintf(topic, "%s/%s/%s", topicPrefix, SUFFIX_LOG, LOG_END[level]);
	publish(topic, message.c_str());
}


void Mqtt::GetConfiguration() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	char topic[TOPIC_LENGTH];
	Debug2("Configuration:", buf);
	Config.IsConfigReady = false;
	sprintf(topic, "%s/%s", topicPrefix, SUFFIX_CONFIG_REQUEST);
	Debug2("Request:", topic);
	publish(topic, buf);
}

void Mqtt::PublishUnit(const Unit *unit) {
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
	}
	sprintf(topic, "%s/%s/%c%02X", topicPrefix, middle, unit->Type, unit->Id);
	char payload[PAYLOAD_LENGTH];
	sprintf(payload, "%s", (unit->status ? "true" : "false"));
	Debug_("Publish:[");
	Debug_(topic);
	Debug_("]:");
	Debug(payload);
	MqttClient.publish(topic, payload);
}

void Mqtt::GetActions() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	char topic[TOPIC_LENGTH];
	Debug2("Action:", buf);
	Config.IsConfigReady = false;
	sprintf(topic, "%s/%s", topicPrefix, SUFFIX_ACTIONS_REQUEST);
	Debug2("Request:", topic);
	publish(topic, buf);
}
void Mqtt::SubscribeUnits() {
	char topic[TOPIC_LENGTH];
	Debug("Subscribing Units...");
	for (int i = 0; i < Config.numberUnits; i++) {

		switch (Config.units[i]->Type) {
			case UnitType::BUTTON: {
				sprintf(topic, "%s/%s", topicPrefix, SUFFIX_GET_BUTTONS);
				break;
			}
			case UnitType::RELAY: {
				sprintf(topic, "%s/%s", topicPrefix, SUFFIX_GET_RELAYS);
				break;
			}
		}
		sprintf(topic, "%s/%c%02X", topic, Config.units[i]->Type, Config.units[i]->Id);
		Debug2("Subscription:", topic);
		subscribe(topic);
	}
}