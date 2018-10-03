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
/*
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
*/
	if (serialNumber != -1) {
		HardwareSerial *port = NULL;
		switch (serialNumber) {
		case 0:
			port = &Serial;
			break;
		case 1:
			port = &Serial1;
			break;
		case 2:
			port = &Serial2;
			break;
		case 3:
			port = &Serial3;
			break;
		default:
			Loger::Debug("Port not found");
			port = NULL;
			break;
		}
		if (port != NULL) {
			pzem = new PZEM004T(port);
		}
	}
	else {
		if (serialTX != -1 && serialRX != -1) {
			pzem = new PZEM004T(serialRX, serialTX);
		}
	}
	if (pzem!=NULL) {
		//IPAddress ip = IPAddress(10, 10, 10, 10);
		pzem->setAddress(ip);
		MqttClient.PublishUnit(this);
	}
}

double PowerMeter::current() {
	if (pzem != NULL) {
		return factor * pzem->current(ip);
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
		return factor * pzem->power(ip);
	}
	else {
		return 0.0;
	}
}
double PowerMeter::energy() {
	if (pzem != NULL) {
		return factor * pzem->energy(ip);
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
	char payload[PAYLOAD_LENGTH];
	
	double v;

	v = voltage();
	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_VOLTAGE);
	MqttClient.Publish(topic, payload);

	v = current();
	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_CURRENT);
	MqttClient.Publish(topic, payload);

	v = power();
	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_POWER);
	MqttClient.Publish(topic, payload);

	v = energy();
	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_ENERGY);
	MqttClient.Publish(topic, payload);
}

void PowerMeter::MqttTopic(uint16_t unitId, char* topic,PowerMeterValues val) {
	char topic0[TOPIC_LENGTH];
	sprintf(topic0, "%s%s%c%04d", MQTT_POWERMETER, MQTT_SEPARATOR, UnitType::POWER_METER, unitId);

	switch (val) {
	case PM_VOLTAGE:
		sprintf(topic, "%s%sVoltage", topic0, MQTT_SEPARATOR);
		break;
	case PM_CURRENT:
		sprintf(topic, "%s%sCurrent", topic0, MQTT_SEPARATOR);
		break;
	case PM_POWER:
		sprintf(topic, "%s%sPower", topic0, MQTT_SEPARATOR);
		break;
	case PM_ENERGY:
		sprintf(topic, "%s%sEnergy", topic0, MQTT_SEPARATOR);
		break;
	default:
		topic[0] = 0;
		break;
	}
}

void PowerMeter::SetDefault() {
}

void PowerMeter::FinalInitUnit() {

}

void PowerMeter::ProcessUnit(ActionType action) {
}
