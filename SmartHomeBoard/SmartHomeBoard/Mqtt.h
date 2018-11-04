// mqtt.h

#pragma once 
/*
#ifdef MQTT_MAX_PACKET_SIZE
#undef MQTT_MAX_PACKET_SIZE
#endif
#define MQTT_MAX_PACKET_SIZE 256
*/

#include <PubSubClient.h>
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
#include "definitions.h"
#include "configuration.h"
#include "ext_global.h"


//#include "types.h"


#define MQTT_RECONNECT_TIME	10000
#define TOPIC_LENGTH	100
#define PAYLOAD_LENGTH	100
#define MQTT_TRY_COUNT 5
#define MQTT_WAITING_RESPONSE 10000
#define MQTT_RESUBSCRIBE_TRY_COUNT 5
#define MQTT_RESUBSCRIPTION_DELAY	50


#define BOARD_ID "Board_%02d"
#define MQTT_SEPARATOR "/"

#define CHECK_MQTT	"info"

#define MQTT_WATCH_DOG "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "WatchDog"
#define MQTT_CONFIG_REQUEST "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "ConfigurationRequest"
#define MQTT_CONFIG_RESPONSE "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "Configuration"
#define MQTT_ACTIONS_REQUEST "Config" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "ActionRequest"
#define MQTT_ACTIONS_RESPONSE "Config" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "Action"
#define MQTT_RESET_BOARD "Config" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "Reset"
#define MQTT_EQUIPMENT "Equipment" 
#define MQTT_VIRTUAL_EQUIPMENT MQTT_EQUIPMENT MQTT_SEPARATOR "Virtual"
#define MQTT_LOG  "Logs" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "%s"
#define MQTT_BUTTONS MQTT_EQUIPMENT MQTT_SEPARATOR "Buttons"
#define MQTT_RELAYS  MQTT_EQUIPMENT MQTT_SEPARATOR "Relays"
#define MQTT_1WIREBUS  MQTT_EQUIPMENT MQTT_SEPARATOR "1-WireBuses"
#define MQTT_1WIRETHERMO  MQTT_EQUIPMENT MQTT_SEPARATOR "Thermo"
#define MQTT_POWERMETER  MQTT_EQUIPMENT MQTT_SEPARATOR "PowerMeter"
#define MQTT_VIRTUAL_BUTTONS  MQTT_VIRTUAL_EQUIPMENT MQTT_SEPARATOR "Buttons"


void callbackFunc(char* topic, uint8_t* payload, unsigned int length);

class Mqtt : public PubSubClient
{
	public:
		unsigned long mqttWaiting;
		void InitMqtt();
		void MqttLoop();
		void GetConfiguration();
		void GetActions();
		void PublishLog(DebugLevel level, String message);
		void PublishUnit(const Unit* unit);
		void SubscribeUnits();
		void SubscribeUnit(int unitNumber);
		void Subscribe(const char* topic);
		bool Publish(const char* topic, const char* payload);

		Mqtt();
		void Callback(char* topic, uint8_t* payload, unsigned int length);
		void WatchDog();

private:
		char *boardId = (char*)BOARD_ID;
		const char *LOG_END[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };
		
		bool MqttReconnect();
//		void SetTopicNames();
		bool firstConnect = true;

};

