// 
// 
// 
/*
Name:		Arduino.ino
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/
//#include "PubSubClient.h"
#include "mqtt.h"
#include "Loger.h"
#include "definitions.h"
//#include "ext_global.h"
//#include "process.h"
//#include "button.h"
//#include "light.h"
//#include "relay.h"

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
/*
void MessageButton(String topic, String payload) {
	String deviceId = GetDeviceId(topic);
	if (!deviceId.equals("")) {
		ButtonUnit *unit = FindButton(deviceId.c_str());
		byte oldValue = unit->status;
		unit->status = (ButtonStatus)ToInt(payload);
		if (unit->status != oldValue)
			ProcessAction(unit->Id, unit->status, unit->status, oldValue);
		//Debug("Point 1.0.1");
	}
}

void MessageLight(String topic, String payload) {
	String deviceId = GetDeviceId(topic);
	if (!deviceId.equals("")) {
		LightUnit *unit = FindLight(deviceId.c_str());
		byte oldValue = unit->status;
		unit->status = (bool)ToInt(payload);
		if (unit->status != oldValue)
			ProcessAction(unit->Id, unit->status, unit->status, oldValue);
	}

}

void MessageRelay(String topic, String payload) {
	String deviceId = GetDeviceId(topic);
	if (!deviceId.equals("")) {
		RelayUnit *unit = FindRelay(deviceId.c_str());
		byte oldValue = unit->status;
		unit->status = (bool)ToInt(payload);
		if (unit->status != oldValue) {
			Debug_("Status:");
			Debug(unit->status);
			RelaySet(unit, unit->status);
			ProcessAction(unit->Id, unit->status, unit->status, oldValue);
		}
	}

}

void SubscribeButtons() {
	for (int i = 0; i < NUMBER_OF_BUTTONS && &(Buttons[i]) != NULL; i++) {
		if (Buttons[i].Id[0] != 0) {
			char str[100];
			sprintf(str, "%s/%s",TOPIC_SUBSCRIPTION_BUTTONS, Buttons[i].Id);
			MqttClient.subscribe(str);
		}
	}
}

void SubscribeRelays() {
	for (int i = 0; i < NUMBER_OF_RELAYS && &(Relays[i]) != NULL; i++) {
		if (Relays[i].Id[0] != 0) {
			char str[100];
			sprintf(str, "%s/%s", TOPIC_SUBSCRIPTION_RELAYS, Relays[i].Id);
			MqttClient.subscribe(str);
		}
	}
}
void SubscribeLigts() {
	for (int i = 0; i < NUMBER_OF_LIGHTS && &(Buttons[i]) != NULL; i++) {
		if (Lights[i].Id[0] != 0) {
			char str[100];
			sprintf(str, "%s/%s", TOPIC_SUBSCRIPTION_LIGHTS, Buttons[i].Id);
			MqttClient.subscribe(str);
		}
	}
}


void PublishButton(ButtonUnit unit) {
	char topic[100];
	sprintf(topic, "%s/%s", TOPIC_PUBLISH_BUTTONS, unit.Id);
	char payload[4];
	sprintf(payload, "%u", unit.status);
	Debug_("Publish:[");
	Debug_(topic);
	Debug_("]:");
	Debug(payload);

	MqttClient.publish(topic, payload,3);
}

void PublishRelay(RelayUnit unit) {
	char topic[100];
	sprintf(topic, "%s/%s", TOPIC_PUBLISH_RELAYS, unit.Id);
	char payload[6];
	sprintf(payload, "%s", (unit.status? "true" : "false"));
	Debug_("Publish:[");
	Debug_(topic);
	Debug_("]:");
	Debug(payload);
	MqttClient.publish(topic, payload,5);
}

void PublishLight(LightUnit unit) {
	char topic[100];
	sprintf(topic, "%s/%s", TOPIC_PUBLISH_LIGHTS, unit.Id);
	char payload[6];
	sprintf(payload, "%s", (unit.status ? "true" : "false"));
	Debug_("Publish:[");
	Debug_(topic);
	Debug_("]:");
	Debug(payload);
	MqttClient.publish(topic, payload,5);
}
*/

bool Mqtt::MqttReconnect() {
	//Если соединение MQTT неактивно, то пытаемся установить его и опубликовать/подписаться
	static unsigned long errorTime = 0;

	if (!connected()) {
		if (errorTime == 0 || errorTime + timeout < millis()) { //reconnect
			
			SerialLog(D_WARN, "Connect to MQTT-broker...  ");
			//Подключаемся и подписываемся
			if (connect(TOPIC_PREFIX)) {
				//Log(D_INFO, "MQTT connected");
				errorTime = 0;
				//Подписываемся на все темы
				//MqttClient.subscribe(TOPIC_CONFIG_LENGTH);
				Debug2("Subscribe:", TOPIC_CONFIG_RESPONSE);
				subscribe(TOPIC_CONFIG_RESPONSE);
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
	/*
	if (strTopic.startsWith(TOPIC_CONFIG_LENGTH)) {
		Config.setNumberUnits(strPayload.toInt());
	}
	else*/ if (strTopic.startsWith(TOPIC_CONFIG_RESPONSE)) {
		Config.updateConfig(strPayload.c_str());
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
	int ind = strlen(TOPIC_PREFIX) - 2;
	Debug2("Ind:", ind);
	sprintf(buf, "%02X", Config.BoardId);
	TOPIC_PREFIX[ind] = buf[0];
	TOPIC_PREFIX[ind+1] = buf[1];
	Debug2("TOPIC_PREFIX", TOPIC_PREFIX);
//TOPIC_CONFIG_LENGTH[ind] = buf[0];
	//TOPIC_CONFIG_LENGTH[ind+1] = buf[1];
	TOPIC_CONFIG_REQUEST[ind] = buf[0];
	TOPIC_CONFIG_REQUEST[ind + 1] = buf[1];
	TOPIC_CONFIG_RESPONSE[ind] = buf[0];
	TOPIC_CONFIG_RESPONSE[ind + 1] = buf[1];
	TOPIC_LOG[ind] = buf[0];
	TOPIC_LOG[ind + 1] = buf[1];

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
void Mqtt::PublishLog(DebugLevel level, String message){
	char topic[200];
	sprintf(topic, "%s/%s", TOPIC_LOG, LOG_SUFFIX[level]);
	publish(topic, message.c_str());
}


void Mqtt::GetConfiguration() {
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
	Debug2("Buf:", buf);
	Config.IsConfigReady = false;
	publish(TOPIC_CONFIG_REQUEST, buf );
}