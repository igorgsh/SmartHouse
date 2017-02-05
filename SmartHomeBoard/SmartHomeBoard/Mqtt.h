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

#define TOPIC_HEAD "Arduino_##"
#define TOPIC_TOSERVER "ToServer"
#define TOPIC_TOCU "ToCU"
#define MQTT_RECONNECT_TIME	10000
#define TOPIC_LENGTH	100
#define PAYLOAD_LENGTH	100
#define SUFFIX_CONFIG_REQUEST "Config/" TOPIC_TOSERVER  "/ConfigurationRequest"
#define SUFFIX_CONFIG_RESPONSE "Config/" TOPIC_TOCU  "/Configuration"
#define SUFFIX_ACTIONS_REQUEST "Config/" TOPIC_TOSERVER  "/ActionRequest"
#define SUFFIX_ACTIONS_RESPONSE "Config/" TOPIC_TOCU  "/Action"
#define SUFFIX_UNITS_TOCU "Units/" TOPIC_TOCU
#define SUFFIX_UNITS_TOSERVER "Units/" TOPIC_TOSERVER 
#define SUFFIX_LOG  "Log"
#define SUFFIX_PUT_BUTTONS SUFFIX_UNITS_TOSERVER "/Buttons"
#define SUFFIX_PUT_RELAYS  SUFFIX_UNITS_TOSERVER "/Relays"
#define SUFFIX_PUT_1WIREBUS  SUFFIX_UNITS_TOSERVER "/1-WireBuses"
#define SUFFIX_PUT_1WIRETHERMO  SUFFIX_UNITS_TOSERVER "/Thermo"
#define SUFFIX_GET_BUTTONS SUFFIX_UNITS_TOCU "/Buttons"
#define SUFFIX_GET_RELAYS  SUFFIX_UNITS_TOCU "/Relays"
#define SUFFIX_GET_1WIREBUS  SUFFIX_UNITS_TOCU "/1-WireBuses"
#define SUFFIX_GET_1WIRETHERMO  SUFFIX_UNITS_TOCU "/Thermo"

void callbackFunc(char* topic, uint8_t* payload, unsigned int length);

class Mqtt : public PubSubClient
{
	public:

		void InitMqtt();
		void MqttLoop();
		void GetConfiguration();
		void GetActions();
		void PublishLog(DebugLevel level, String message);
		void PublishUnit(const Unit* unit);
		void SubscribeUnits();


		Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort,callbackFunc, EthClient) {};
		void Callback(char* topic, uint8_t* payload, unsigned int length);

private:
		char *topicPrefix = (char*)TOPIC_HEAD;
		const char *LOG_END[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };
		
		bool MqttReconnect();
		void SetTopicNames();

};

