#include "PowerMeter.h"
#include "ext_global.h"
#include "mqtt.h"
#include "SigmaEEPROM.h"

extern Mqtt MqttClient;


PowerMeter::PowerMeter()
{
}


PowerMeter::~PowerMeter()
{
}

void PowerMeter::InitUnit() {

	if (serialNumber != 0) {
		HardwareSerial *port = NULL;
		switch (serialNumber) {
//		case 0:
//			port = &Serial;
//			break;
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
		if (serialTX != 0 && serialRX != 0) {
			pzem = new PZEM004T(serialRX, serialTX);
		}
	}
	if (pzem!=NULL) {
		pzem->setAddress(ip);
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
	char topic[MQTT_TOPIC_LENGTH];
	char payload[MQTT_PAYLOAD_LENGTH];

	double v;

	v = voltage();
	if (v <= 0) v = 0;

	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_VOLTAGE);
	MqttClient.Publish(topic, payload);

	v = current();
	if (v <= 0) v = 0;
	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_CURRENT);
	MqttClient.Publish(topic, payload);

	v = power();
	if (v <= 0) v = 0;
	dtostrf(v, 6, 2, payload);
	MqttTopic(Id, topic, PM_POWER);
	MqttClient.Publish(topic, payload);

	v = energy();
	if (v > 0) {
		dtostrf(v, 6, 2, payload);
		MqttTopic(Id, topic, PM_ENERGY);
		MqttClient.Publish(topic, payload);
	}
}


void PowerMeter::MqttTopic(uint16_t unitId, char* topic,PowerMeterValues val) {
	char topic0[MQTT_TOPIC_LENGTH];
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


bool PowerMeter::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::POWER_METER) return false;
	PowerMeter* tu = (PowerMeter*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		serialRX == tu->serialRX &&
		serialTX == tu->serialTX &&
		serialNumber == tu->serialNumber &&
		factor == tu->factor
		);
	return res;
}


void PowerMeter::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	serialNumber = SigmaEEPROM::Read8(addr + 2);
	serialRX = SigmaEEPROM::Read8(addr + 3);
	serialTX = SigmaEEPROM::Read8(addr + 4);
	factor = SigmaEEPROM::Read16(addr + 5);
}

void PowerMeter::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, serialNumber);
	SigmaEEPROM::Write8(addr + 3, serialRX);
	SigmaEEPROM::Write8(addr + 4, serialTX);
	SigmaEEPROM::Write16(addr + 5, factor);

}

void PowerMeter::ConfigField(const JsonObject& jsonList) {

	if (jsonList.containsKey(F("Serial"))) {
		serialNumber = jsonList[F("Serial")];
	}

	if (jsonList.containsKey(F("SerialRX"))) {
		serialRX = jsonList[F("SerialRX")];
	}

	if (jsonList.containsKey(F("SerialTX"))) {
		serialTX = jsonList[F("SerialTX")];
	}

	if (jsonList.containsKey(F("Factor"))) {
		factor = jsonList[F("Factor")];
	}
}


void const PowerMeter::print(const char* header, DebugLevel level) {
	String str0 = "";

	if (header != NULL) {
		str0 = header;
	}
	str0 += F("Id:");
	str0 += (unsigned int)Id;
	str0 += F(";Type:");
	str0 += (char)Type;
	str0 += F(";Serial:");
	str0 += (unsigned int)Serial;
	str0 += F(";SerialRX:");
	str0 += (unsigned int)serialRX;
	str0 += F(";SerialTX:");
	str0 += (unsigned int)serialTX;
	str0 += F(";Factor:");
	str0 += (unsigned int)factor;
	str0 += F(";subscription:");
	str0 += (isSubscribed ? F("true") : F("false"));
	str0 += F("@");
	Loger::Log(level, str0);
}
