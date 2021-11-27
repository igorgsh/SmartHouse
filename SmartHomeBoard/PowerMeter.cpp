#include "PowerMeter.h"
#include "Configuration.h"
#include "SigmaEEPROM.h"

extern Configuration Config;


PowerMeter::PowerMeter()
{
}


PowerMeter::~PowerMeter()
{
}

void PowerMeter::InitUnit(bool isParent) {

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
			Config.Log->Error(F("Port not found"));
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
		//IPAddress ip = IPAddress(10, 10, 10, 10);
		pzem->setAddress(ip);
		//MqttClient.PublishUnit(this);
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

void PowerMeter::UnitLoop(unsigned long timePeriod, bool isParent, bool val) {
	//Parent is impossible

	double v;
	if (timePeriod == 1000) {
		//Config.Log->append("PWR: id=").append(Id).append("; time = ").append(timePeriod).append("; step=").append(step).Debug();
		if (step == PM_VOLTAGE) {
			v = voltage();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_CURRENT;
		}
		else if (step == PM_CURRENT) {
			v = current();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_POWER;
		}
		else if (step == PM_POWER) {
			v = power();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_ENERGY;
		}
		else if (step == PM_ENERGY) {
			v = energy();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_VOLTAGE;
		}
	}
}

void PowerMeter::PublishPowerMeter(PowerMeterValues step, double v) {
	char topic[MQTT_TOPIC_LENGTH];
	char payload[MQTT_PAYLOAD_LENGTH];
	//Config.Log->append("PWR:val=").append(v).Debug();
	//sprintf(payload, "%f", v);
	//dtostrf(v, 20, 2, payload);
	unsigned long p1;
	uint8_t p2;

	p1 = (unsigned long)v;
	p2 = (uint8_t)((v - p1) * 100);
	sprintf(payload, "%lu.%02u", p1, p2);

	MqttTopic(Id, topic, step);
	Config.MqttClient->Publish(topic, payload);

}


void PowerMeter::MqttTopic(uint16_t unitId, char* topic,PowerMeterValues step) {
	char topic0[MQTT_TOPIC_LENGTH];
	sprintf(topic0, "%s%s%c%04d", MQTT_POWERMETER, MQTT_SEPARATOR, UnitType::POWER_METER, unitId);

	switch (step) {
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


void PowerMeter::FinalInitUnit(bool isParent) {
	// No Subscription
	// No value request
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

	if (jsonList.containsKey("Serial")) {
		serialNumber = jsonList["Serial"];
	}

	if (jsonList.containsKey("SerialRX")) {
		serialRX = jsonList["SerialRX"];
	}

	if (jsonList.containsKey("SerialTX")) {
		serialTX = jsonList["SerialTX"];
	}

	if (jsonList.containsKey("Factor")) {
		factor = jsonList["Factor"];
	}
}


void const PowerMeter::print(const char* header, DebugLevel level) {

	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F("Id:")).append((unsigned int)Id);
	Config.Log->append(F(";Type:")).append((char)Type);
	Config.Log->append(F(";Serial:")).append((unsigned int)Serial);
	Config.Log->append(F(";SerialRX:")).append((unsigned int)serialRX);
	Config.Log->append(F(";SerialTX:")).append((unsigned int)serialTX);
	Config.Log->append(F(";Factor:")).append((unsigned int)factor);
//	Log.append(F(";subscription:")).append(isSubscribed ? "true" : "false");
	Config.Log->append(F(" @"));
	Config.Log->Log(level);
}
