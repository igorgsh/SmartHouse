// 
// 
// 
/*
Name:		Arduino.ino
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/
#include "PubSubClient.h"
#include "mqtt.h"
#include "ext_global.h"
#include "process.h"
#include "button.h"
#include "light.h"
#include "relay.h"



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
		if (unit->status != oldValue)
			ProcessAction(unit->Id, unit->status, unit->status, oldValue);
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


bool MqttReconnect() {
	//���� ���������� MQTT ���������, �� �������� ���������� ��� � ������������/�����������
	static unsigned long errorTime = 0;

	if (!MqttClient.connected()) {
		if (errorTime == 0 || errorTime + 10000 < millis()) { //reconnect
			Log(D_INFO, "Connect to MQTT-broker...  ");
			//������������ � �������������
			if (MqttClient.connect(TOPIC_PREFIX)) {
				Log(D_INFO, "MQTT connected");
				errorTime = 0;
				//������������� �� ��� ����
				SubscribeButtons();
				Debug("Buttons subscribed");
				SubscribeLigts();
				Debug("Lights subscribed");
				SubscribeRelays();
				Debug("Relays subscribed");
				//Receive all messages
				MqttClient.loop();
			}
			else {
				//���� �� ������������, ���� 10 ������ � �������� �����
				Log_(D_ERROR, "Failed, rc=");
				Log2_(D_ERROR, MqttClient.state(), DEC);
				Log(D_ERROR, " try again in 10 seconds");
				errorTime = millis();
			}
		}
	}

	return (errorTime == 0);
}

void callback(char* topic, byte* payload, unsigned int length) {
	//����������� ����(topic) � �������� (payload) � ������
	payload[length] = '\0';
	String strTopic = String(topic);
	String strPayload = String((char*)payload);
	//��������� ��� "���������" �� ������� �� ��������:
	Debug_("[");
	Debug_(strTopic);
	Debug_("]:");
	Debug(strPayload);

	if (strTopic.startsWith(TOPIC_SUBSCRIPTION_BUTTONS)) {
		MessageButton(strTopic, strPayload);
		//Debug("Point 1.1")
	}
	else if (strTopic.startsWith(TOPIC_SUBSCRIPTION_LIGHTS)) {
		MessageLight(strTopic, strPayload);
		//Debug("Point 1.2")
	}
	else if (strTopic.startsWith(TOPIC_SUBSCRIPTION_RELAYS)) {
		MessageRelay(strTopic, strPayload);
		//Debug("Point 1.3")
	}
	//Debug("Point 2");
}
void InitMqtt(void) {
	//MqttClient = new PubSubClient(MqttServer, MqttPort, callback, EthClient);
	MqttReconnect();
}

void MqttLoop() {
	if (MqttReconnect()) {
		MqttClient.loop();
	}
}
