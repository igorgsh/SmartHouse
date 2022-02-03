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
		switch (serialNumber) {
//		case 0:
//			port = &Serial;
//			break;
		case 1:
			hardPort = &Serial1;
			break;
		case 2:
			hardPort = &Serial2;
			break;
		case 3:
			hardPort = &Serial3;
			break;
		default:
			Config.Log->Error(F("Port not found"));
			break;
		}
		if (hardPort != NULL) {
			if (version != 3) {
				pzem = new PZEM004T(hardPort);
				pzem->setAddress(ip);
			}
			else {
				//pzem3 = new PZEM004Tv30(*hardPort);
			}
		}

	}
	else {
		if (serialTX != 0 && serialRX != 0) {
			if (version != 3) {
				pzem = new PZEM004T(serialRX, serialTX);
				pzem->setAddress(ip);
			}
			else {
				softPort = new SoftwareSerial(serialRX, serialTX);
				softPort->begin(9600);

				//Config.Log->append("SoftPort=").append(softPort->available()).Debug();
				pzem3 = new PZEM004Tv30(*softPort);
			}
		}
	}
}

double PowerMeter::Current() {
	double v;
	if (version != 3) {
		v = pzem->current(ip);
	}
	else {
		v = pzem3->current();
	}
	if (v == NAN) {
		v = 0.0;
	}
	return v;
}


double PowerMeter::Voltage() {
	double v;
	if (version != 3) {
		v = pzem->voltage(ip);
	}
	else {
		v = pzem3->voltage();
	}
	if (v == NAN) {
		v = 0.0;
	}
	return v;
}

double PowerMeter::Power() {
	double v;
	if (version != 3) {
		v = pzem->power(ip);
	}
	else {
		v = pzem3->power();
	}
	if (v == NAN) {
		v = 0.0;
	}
	return v;
}
double PowerMeter::Energy() {
	double v;
	if (version != 3) {
		v = pzem->energy(ip)/1000.0;
	}
	else {
		v = pzem3->energy();
	}
	if (v == NAN) {
		v = 0.0;
	}
	return v;
}

double PowerMeter::Frequency()
{
	double v;
	if (version != 3) {
		v = 0.0;
	}
	else {
		v = pzem3->frequency();
	}
	if (v == NAN) {
		v = 0.0;
	}
	return v;
}

double PowerMeter::PowerFactor()
{
	double v;
	if (version != 3) {
		v = 0.0;
	}
	else {
		v = pzem3->pf();
		//Config.Log->append("PF:").append(v).Debug();
	}
	if (v == NAN) {
		v = 0.0;
	}
	return v;
}

void PowerMeter::UnitLoop(unsigned long timePeriod, bool isParent, bool val) {
	//Parent is impossible

	double v;
	if (timePeriod == 1000) {
		//Config.Log->append("PWR: id=").append(Id).append("; time = ").append(timePeriod).append("; step=").append(step).Debug();
		if (step == PM_VOLTAGE) {
			v = Voltage();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_CURRENT;
		}
		else if (step == PM_CURRENT) {
			v = Current();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_POWER;
		}
		else if (step == PM_POWER) {
			v = Power();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_ENERGY;
		}
		else if (step == PM_ENERGY) {
			v = Energy();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_FREQUENCY;
		}
		else if (step == PM_FREQUENCY) {
			v = Frequency();
			if (v <= 0) v = 0;
			PublishPowerMeter(step, v);
			step = PM_POWERFACTOR;
		}
		else if (step == PM_POWERFACTOR) {
			v = PowerFactor();
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
	unsigned int p2;

	p1 = (unsigned long)v;
	p2 = (unsigned int)((v - p1) * 1000);
	sprintf(payload, "%lu.%03u", p1, p2);

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
	case PM_FREQUENCY:
		sprintf(topic, "%s%sFrequency", topic0, MQTT_SEPARATOR);
		break;
	case PM_POWERFACTOR:
		sprintf(topic, "%s%sPowerFactor", topic0, MQTT_SEPARATOR);
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
		version == tu->version
		);
	return res;
}


void PowerMeter::ReadFromEEPROM(uint16_t addr) {

	Id = SigmaEEPROM::Read16(addr);
	Type = SigmaEEPROM::Read8(addr + 2);
	serialNumber = SigmaEEPROM::Read8(addr + 3);
	serialRX = SigmaEEPROM::Read8(addr + 4);
	serialTX = SigmaEEPROM::Read8(addr + 5);
	version = SigmaEEPROM::Read8(addr + 6);
}

void PowerMeter::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write16(addr, Id);
	SigmaEEPROM::Write8(addr + 2, Type);
	SigmaEEPROM::Write8(addr + 3, serialNumber);
	SigmaEEPROM::Write8(addr + 4, serialRX);
	SigmaEEPROM::Write8(addr + 5, serialTX);
	SigmaEEPROM::Write8(addr + 6, version);

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
	if (jsonList.containsKey("version")) {
		version = jsonList["version"];
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
	Config.Log->append(F(";Version:")).append((unsigned int)version);
//	Log.append(F(";subscription:")).append(isSubscribed ? "true" : "false");
	Config.Log->append(F(" @"));
	Config.Log->Log(level);
}
