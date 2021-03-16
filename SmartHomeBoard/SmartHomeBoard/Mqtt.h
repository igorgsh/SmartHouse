// mqtt.h

#pragma once 

#include <PubSubClient.h>
#include "definitions.h"
#include "configuration.h"
#include "ext_global.h"



#define MQTT_INITIAL_RETRY_DELAY	1000 //delay between INITIAL reconnection retry
#define MQTT_RETRY_TIME				10000 //delay between mqtt reconnect in loop
#define MQTT_TOPIC_LENGTH	100
#define MQTT_PAYLOAD_LENGTH	100
#define MQTT_TRY_COUNT 5
#define MQTT_WAITING_RESPONSE 10000
#define MQTT_RESUBSCRIBE_TRY_COUNT 1
#define MQTT_RESUBSCRIPTION_DELAY	50
#define MQTT_ETHERNET_TIME (5*1000)


#define BOARD_ID "Board_%02d"
#define MQTT_SEPARATOR "/"

#define CHECK_MQTT	"info"

#define MQTT_CONFIGURATION "Configuration"
#define MQTT_ACTIONS "Action"
#define MQTT_EQUIPMENT "Equipment" 


#define MQTT_CONFIG_TOPIC "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR

#define MQTT_WATCH_DOG MQTT_CONFIG_TOPIC "WatchDog"
#define MQTT_CONFIG_REQUEST MQTT_CONFIG_TOPIC "ConfigurationRequest"
#define MQTT_CONFIG_RESPONSE MQTT_CONFIG_TOPIC MQTT_CONFIGURATION
#define MQTT_ACTIONS_REQUEST MQTT_CONFIG_TOPIC "ActionRequest"
#define MQTT_ACTIONS_RESPONSE MQTT_CONFIG_TOPIC MQTT_ACTIONS
#define MQTT_RESET_BOARD MQTT_CONFIG_TOPIC "Reset"
#define MQTT_VIRTUAL_EQUIPMENT MQTT_EQUIPMENT MQTT_SEPARATOR "Virtual"
#define MQTT_LOG  "Logs" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "%s"
#define MQTT_BUTTONS MQTT_EQUIPMENT MQTT_SEPARATOR "Buttons"
#define MQTT_RELAYS  MQTT_EQUIPMENT MQTT_SEPARATOR "Relays"
#define MQTT_1WIREBUS  MQTT_EQUIPMENT MQTT_SEPARATOR "1-WireBuses"
#define MQTT_1WIRETHERMO  MQTT_EQUIPMENT MQTT_SEPARATOR "Thermo"
#define MQTT_POWERMETER  MQTT_EQUIPMENT MQTT_SEPARATOR "PowerMeter"
#define MQTT_VIRTUAL_BUTTONS  MQTT_VIRTUAL_EQUIPMENT MQTT_SEPARATOR "Buttons"
#define MQTT_CONTACTOR  MQTT_EQUIPMENT MQTT_SEPARATOR "Contactors"
#define MQTT_SHIFT_REGISTER_IN  MQTT_EQUIPMENT MQTT_SEPARATOR "ShiftRegisterIn"
#define MQTT_SHIFT_REGISTER_OUT  MQTT_EQUIPMENT MQTT_SEPARATOR "ShiftRegisterOut"


void callbackFunc(char* topic, uint8_t* payload, unsigned int length);

class Mqtt : public PubSubClient
{
	public:
		unsigned long mqttWaiting;
		void InitMqtt();
		void MqttLoop();
		void GetConfiguration();
		void GetActions();
		void PublishLog(DebugLevel level, const String &message);
		void PublishUnit(const Unit* unit);
		void SubscribeUnits();
		void SubscribeUnit(int unitNumber);
		void Subscribe(const String &topic);
		bool Publish(const String &topic, const String &payload);
		void Callback(const String& topic, const String& payload, unsigned int length);

		Mqtt();
		void WatchDog();

private:
		char *boardId = (char*)BOARD_ID;
		const char *LOG_END[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };

		bool MqttReconnect();
		void SubscribeBoardTopic(const char* templ, byte boardId);
		int GetUnitId(const String &topic, int prefixLen);
};
