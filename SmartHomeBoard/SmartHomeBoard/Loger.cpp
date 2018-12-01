#include "Loger.h"
#include "mqtt.h"
#include "definitions.h"

extern Mqtt MqttClient;

void Loger::Log(DebugLevel level, String message) {
	if (level <= dLevel) {
		MqttClient.PublishLog(level, message);
		Serial.println(message);
	}
}