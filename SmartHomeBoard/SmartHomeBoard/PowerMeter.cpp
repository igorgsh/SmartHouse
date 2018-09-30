#include "PowerMeter.h"
#include "ext_global.h"
#include "mqtt.h"

extern Mqtt MqttClient;


PowerMeter::PowerMeter()
{
}


PowerMeter::~PowerMeter()
{
}

void PowerMeter::InitUnit() {
	Loger::Debug("Init Power Meter id=" + String(Id) + "; Pin=" + String(Pin));

	switch (Pin) {
	case 18: //Serial 1
	case 19:
		port = &Serial1;
		break;
	case 16: //Serial 2
	case 17:
		port = &Serial2;
		break;
	case 14: //Serial 3
	case 15:
		port = &Serial3;
		break;
	default:
		Loger::Debug("Port not found");
		port = NULL;
		break;
	}
	if (port!=NULL) {
		pzem = new PZEM004T(port);
		//IPAddress ip(10, 10, 10, 10);
		pzem->setAddress(ip);
		MqttClient.PublishUnit(this);
	}
}

double PowerMeter::current() {
	if (pzem != NULL) {
		return pzem->current(ip);
	}
	else {
		return 0.0;
	}
}


double PowerMeter::voltage() {
	if (pzem != NULL) {
		return pzem->voltage(ip);
	}
	else {
		return 0.0;
	}
}

double PowerMeter::power() {
	if (pzem != NULL) {
		return pzem->power(ip);
	}
	else {
		return 0.0;
	}
}
double PowerMeter::energy() {
	if (pzem != NULL) {
		return pzem->energy(ip);
	}
	else {
		return 0.0;
	}
}

void PowerMeter::UnitLoop() {
	//PublishAll();
}

void PowerMeter::PublishAll() {
	char topic[TOPIC_LENGTH];
	char topic0[TOPIC_LENGTH];
	char payload[PAYLOAD_LENGTH];
	//char str0[10];
	
	sprintf(topic0, "%s%s%c%04d", MQTT_POWERMETER, MQTT_SEPARATOR, Type, Id);
	sprintf(topic, "%s%sVoltage", topic0, MQTT_SEPARATOR);
	double v;
	v = voltage();
	dtostrf(v, 6, 2, payload);
	MqttClient.Publish(topic, payload);
	sprintf(topic, "%s%sCurrent", topic0, MQTT_SEPARATOR);
	v = current();
	dtostrf(v, 6, 2, payload);
	MqttClient.Publish(topic, payload);
	sprintf(topic, "%s%sPower", topic0, MQTT_SEPARATOR);
	v = power();
	dtostrf(v, 6, 2, payload);
	MqttClient.Publish(topic, payload);
	sprintf(topic, "%s%sEnergy", topic0, MQTT_SEPARATOR);
	v = energy();
	dtostrf(v, 6, 2, payload);
	MqttClient.Publish(topic, payload);
}


void PowerMeter::SetDefault() {
}

void PowerMeter::FinalInitUnit() {

}

void PowerMeter::ProcessUnit(ActionType action) {
}
