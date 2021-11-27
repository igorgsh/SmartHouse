#include "Unit.h"
#include "Configuration.h"

extern Configuration Config;


void Unit::FillFrom(const Unit* u) {
	Id = u->Id;
	Type = u->Type;
}

void const Unit::print(const char* header, DebugLevel level){
	
	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F("Id:")).append((unsigned int) Id);
	Config.Log->append(F(";Type:")).append((char)Type);
	Config.Log->append(F(";status:")).append((unsigned int)status);
	Config.Log->append(F(" @"));
	Config.Log->Log(level);
}

void Unit::Publish(const char* uPrefix)
{
	sprintf(Config.MqttClient->topicBuff, "%s%s%c%04d", uPrefix, MQTT_SEPARATOR, Type, Id);
	sprintf(Config.MqttClient->payloadBuff, "%u", status);
	Config.MqttClient->Publish(Config.MqttClient->topicBuff, Config.MqttClient->payloadBuff);
}

void Unit::Subscribe(const char* uPrefix, bool isValueRequested)
{
	if (uPrefix != NULL) {
		sprintf(Config.MqttClient->topicBuff, "%s%s%c%04d", uPrefix, MQTT_SEPARATOR, Type, Id);
		Config.MqttClient->Subscribe(Config.MqttClient->topicBuff);

		if (isValueRequested) {
			Config.MqttClient->RequestValue(Config.MqttClient->topicBuff);

		}
	}
}

void Unit::PublishTypedUnit(UnitType ut, uint16_t id, byte status)
{
	const char* unitPrefix;
	byte unitType = ut;
	switch (ut) {
	case UnitType::BUTTON: {
		unitPrefix = MQTT_BUTTONS;
		break;
	}
	case UnitType::RELAY: {
		unitPrefix = MQTT_RELAYS;
		break;
	}
	case UnitType::ONE_WIRE_BUS: {
		unitPrefix = MQTT_1WIREBUS;
		break;
	}
	case UnitType::ONE_WIRE_THERMO: {
		unitPrefix = MQTT_1WIRETHERMO;
		break;
	}
	case UnitType::VIRTUAL_BUTTON: {
		unitPrefix = MQTT_VIRTUAL_BUTTONS;
		unitType = UnitType::BUTTON;
		break;
	}
	case UnitType::CONTACTOR: {
		unitPrefix = MQTT_CONTACTOR;
		break;
	}
	case POWER_METER:
	case NOTHING:
	case SHIFTIN:
	case SHIFTOUT: {
		ut = NOTHING;
		break;
	}
	}
	if (ut != NOTHING) {
		sprintf(Config.MqttClient->topicBuff, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, unitType, id);
		sprintf(Config.MqttClient->payloadBuff, "%u", status);
		Config.MqttClient->Publish(Config.MqttClient->topicBuff, Config.MqttClient->payloadBuff);
	}
}

