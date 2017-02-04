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
#include "utils.h"

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
	Debug2("Point6.4.2:", memoryFree());

	payload[length] = '\0';
	String strTopic = String(topic);
	String strPayload = String((char*)payload);
	//Исследуем что "прилетело" от сервера по подписке:
	Debug_("[");
	Debug_(strTopic);
	Debug_("]:");
	Debug(strPayload);
	Debug2("Point6.4.4:", memoryFree());
	if (strTopic.startsWith((String)(SUFFIX_CONFIG_RESPONSE), strlen(topicPrefix) + 1)) {
		Config.UpdateConfig(strPayload.c_str());
	}
	else if (strTopic.startsWith((String)(SUFFIX_ACTIONS_RESPONSE), strlen(topicPrefix) + 1)) {
		Config.UpdateActions(strPayload.c_str());
	}
	else if (strTopic.startsWith((String)(SUFFIX_GET_BUTTONS), strlen(topicPrefix) + 1)) {
		Config.UpdateButton(strTopic.substring(strlen(topicPrefix) + 1 + strlen(SUFFIX_GET_BUTTONS)+2), strPayload);
	}
	else if (strTopic.startsWith((String)(SUFFIX_GET_RELAYS), strlen(topicPrefix) + 1)) {
		Config.UpdateRelay(strTopic.substring(strlen(topicPrefix) + 1 + strlen(SUFFIX_GET_RELAYS)+2), strPayload);
	}
	Debug2("Point6.4.6:", memoryFree());

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
	int ind = strlen(topicPrefix) - 2;
	topicPrefix[ind] = (int)'0' + (int)((Config.BoardId >> 4) & 0x0F);
	topicPrefix[ind+1] = (int)'0' + (int)(Config.BoardId & 0x0F);
}

void Mqtt::InitMqtt(void) {
	Debug2("Point5.2:", memoryFree());

	SetTopicNames();
	Debug2("Point5.4:", memoryFree());
	MqttReconnect();
	Debug2("Point5.6:", memoryFree());
}

void Mqtt::MqttLoop() {
	Debug2("PointLoop Start", memoryFree());

	if (MqttReconnect()) {
		loop();
	}
	Debug2("PointLoop Finish", memoryFree());

}
void Mqtt::PublishLog(DebugLevel level, String message) {
	char topic[TOPIC_LENGTH];
	sprintf(topic, "%s/%s/%s", topicPrefix, SUFFIX_LOG, LOG_END[level]);
	publish(topic, message.c_str());
}


void Mqtt::GetConfiguration() {
	//char buf[10];
	//Debug2("Point6.2.2:", memoryFree());

	uint8_t rnd = random(0, 1000);
	//sprintf(buf, "%lu", random(0, 1000));
	//Debug2("Point6.2.4:", memoryFree());
	char topic[TOPIC_LENGTH];
	//Debug2("Configuration:", buf);
	Config.IsConfigReady = false;
	//Debug2("Point6.2.6:", memoryFree());
	strcpy(topic, topicPrefix);
	strcat(topic,"/");
	strcat(topic, SUFFIX_CONFIG_REQUEST);
	//sprintf(topic, "%s/%s", topicPrefix, SUFFIX_CONFIG_REQUEST);
	//Debug2("Point6.2.8:", memoryFree());
	Debug2("Request:", topic);
	//publish(topic, buf);
	publish(topic, &rnd, 3);
	//Debug2("Point6.2.10:", memoryFree());

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
	sprintf(payload, "%u", unit->status);
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