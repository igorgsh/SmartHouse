#include "VoltmeterDC.h"
#include "Configuration.h"
#include "SigmaEEPROM.h"

extern Configuration Config;


VoltmeterDC::VoltmeterDC() {

}

VoltmeterDC::~VoltmeterDC() {

}

void VoltmeterDC::callSensor()
{
	if (version == 3) {
		for (int i = 0; i < 3; i++) {
			voltage[i] = voltmeter3->getVoltage((ina3221_ch_t)i);
			current[i] = voltmeter3->getCurrent((ina3221_ch_t)i);
			currentComp[i] = voltmeter3->getCurrentCompensated((ina3221_ch_t)i);
		}
	}
}

void VoltmeterDC::InitUnit(bool isParent)
{
	if (version == 3) {
		voltmeter3 = new INA3221(address);
		voltmeter3->begin(&Wire);
		voltmeter3->reset();
		voltmeter3->setShuntRes(resistance[0], resistance[1], resistance[2]);
	}
	print("Init", DebugLevel::D_DEBUG);
}

void VoltmeterDC::UnitLoop(unsigned long timePeriod, bool isParent, bool val)
{
	// Parent is prohibited
	if (timePeriod == 1000) {
		callSensor();
		publishVoltmeter();
	}
}

void VoltmeterDC::ProcessUnit(ActionType action)
{
}


bool VoltmeterDC::Compare(const Unit* u) {

	if (u == NULL) return false;
	if (u->Type != UnitType::VOLTMETER) return false;
	VoltmeterDC* tu = (VoltmeterDC*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		version == tu->version &&
		address == tu->address &&
		resistance[0] == tu->resistance[0] &&
		resistance[1] == tu->resistance[1] &&
		resistance[2] == tu->resistance[2]
		);
	return res;
}

void VoltmeterDC::ReadFromEEPROM(uint16_t addr) {
	int n = 0;
	Id = SigmaEEPROM::Read16(addr + n);
	n += 2;
	Type = SigmaEEPROM::Read8(addr + n);
	n += 1;
	address = (ina3221_addr_t)SigmaEEPROM::Read8(addr + n);
	n += 1;
	version = SigmaEEPROM::Read8(addr + n);
	n += 1;
	resistance[0] = SigmaEEPROM::Read16(addr + n);
	n += 2;
	resistance[1] = SigmaEEPROM::Read16(addr + n);
	n += 2;
	resistance[2] = SigmaEEPROM::Read16(addr + n);
	//n += 2;
}

void VoltmeterDC::WriteToEEPROM(uint16_t addr) {

	int n = 0;
	SigmaEEPROM::Write16(addr + n, Id);
	n += 2;
	SigmaEEPROM::Write8(addr + n, Type);
	n += 1;
	SigmaEEPROM::Write8(addr + n, address);
	n += 1;
	SigmaEEPROM::Write8(addr + n, version);
	n += 1;
	SigmaEEPROM::Write16(addr + n, resistance[0]);
	n += 2;
	SigmaEEPROM::Write16(addr + n, resistance[1]);
	n += 2;
	SigmaEEPROM::Write16(addr + n, resistance[2]);
	n += 2;

}

void VoltmeterDC::ConfigField(const JsonObject& jsonList) {

	if (jsonList.containsKey("address")) {
		address = (ina3221_addr_t)((byte)jsonList["address"]);
	}

	if (jsonList.containsKey("r0")) {
		resistance[0] = jsonList["r0"];
	}
	if (jsonList.containsKey("r1")) {
		resistance[1] = jsonList["r1"];
	}
	if (jsonList.containsKey("r2")) {
		resistance[2] = jsonList["r2"];
	}
	if (jsonList.containsKey("version")) {
		version = jsonList["version"];
	}
}

void const VoltmeterDC::print(const char* header, DebugLevel level) {

	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F("Id:")).append((unsigned int)Id);
	Config.Log->append(F(";Type:")).append((char)Type);
	Config.Log->append(F(";address:")).append((byte)address);
	Config.Log->append(F(";R:(")).append(resistance[0]).append(",").append(resistance[1]).append(",").append(resistance[2]).append(")");
	Config.Log->append(F(";Version:")).append((unsigned int)version);
	Config.Log->append(F(" @"));
	Config.Log->Log(level);
}

void VoltmeterDC::publishVoltmeter()
{
	char topic0[MQTT_TOPIC_LENGTH];
	char topic[MQTT_TOPIC_LENGTH];
	char payload[MQTT_PAYLOAD_LENGTH];
	unsigned long p1;
	unsigned int p2;

	sprintf(topic0, "%s%s%c%04d", MQTT_VOLTMETERDC, MQTT_SEPARATOR, UnitType::VOLTMETER, Id);

	if (version == 3) {
		for (int i = 0; i < 3; i++) {

			p1 = (unsigned long)voltage[i];
			p2 = (unsigned int)((voltage[i] - p1) * 10);
			sprintf(payload, "%lu.%01u", p1, p2);

			sprintf(topic, "%s%sCH%1u%sVoltage", topic0, MQTT_SEPARATOR, i, MQTT_SEPARATOR);
			Config.MqttClient->Publish(topic, payload);

			p1 = (unsigned long)current[i];
			p2 = (unsigned int)((current[i] - p1) * 10);
			sprintf(payload, "%lu.%01u", p1, p2);
			sprintf(topic, "%s%sCH%1u%sCurrent", topic0, MQTT_SEPARATOR, i, MQTT_SEPARATOR);
			Config.MqttClient->Publish(topic, payload);

			p1 = (unsigned long)currentComp[i];
			p2 = (unsigned int)((currentComp[i] - p1) * 10);
			sprintf(payload, "%lu.%01u", p1, p2);
			sprintf(topic, "%s%sCH%1u%sCurrentCompensated", topic0, MQTT_SEPARATOR, i, MQTT_SEPARATOR);
			Config.MqttClient->Publish(topic, payload);
		}
	}
}
