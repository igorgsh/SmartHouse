#include "Loger.h"
//#include "mqtt.h"
#include "definitions.h"
#include "Configuration.h"

extern Configuration Config;

<<<<<<< HEAD
void Loger::Log(DebugLevel level, const String &message) {
=======
void Loger::Log(DebugLevel level) {
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	if (level <= dLevel) {
		Serial.println(loger->c_str());
		Config.MqttClient->PublishLog(level, loger->c_str());
	}

	loger->clear();
}

Loger::Loger(int size)
{
	Serial.println("LOGER!");
	this->size = size;

	loger_SAFEBUFFER = new char[size + 1];
	loger = new SafeString(size, loger_SAFEBUFFER, "");
}