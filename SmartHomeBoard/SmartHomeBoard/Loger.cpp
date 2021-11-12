#include "Loger.h"
#include "mqtt.h"
#include "definitions.h"

extern Mqtt MqttClient;

void Loger::Log(DebugLevel level) {
	if (level <= dLevel) {
		Serial.println(loger->c_str());
		//MqttClient.PublishLog(level, loger->c_str());
	}

	loger->clear();
}

Loger::Loger(int size)
{
	this->size = size;

	loger_SAFEBUFFER = new char[size + 1];
	loger = new SafeString(size, loger_SAFEBUFFER, "");
}