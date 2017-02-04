// mqtt.h

#pragma once 
/*
#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
*/

#include <PubSubClient.h>
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
#define SUFFIX_GET_BUTTONS SUFFIX_UNITS_TOCU "/Buttons"
#define SUFFIX_GET_RELAYS  SUFFIX_UNITS_TOCU "/Relays"

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

		/*
		PubSubClient();
		PubSubClient(Client& client);
		PubSubClient(IPAddress, uint16_t, Client& client);
		PubSubClient(IPAddress, uint16_t, Client& client, Stream&);
		PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client);
		PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);
		PubSubClient(uint8_t *, uint16_t, Client& client);
		PubSubClient(uint8_t *, uint16_t, Client& client, Stream&);
		PubSubClient(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client);
		PubSubClient(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);
		PubSubClient(const char*, uint16_t, Client& client);
		PubSubClient(const char*, uint16_t, Client& client, Stream&);
		PubSubClient(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client);
		PubSubClient(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);
*/
		
		Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, &callbackFunc, EthClient) {};
		void Callback(char* topic, uint8_t* payload, unsigned int length);

private:
	//PubSubClient *MqttClient;
		char *topicPrefix = (char*)TOPIC_HEAD;
		const char *LOG_END[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };
//		const int timeout = MQTT_RECONNECT_TIME;
		bool MqttReconnect();
		void SetTopicNames();


//#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)

};

//static char *TOPIC_CONFIG_LENGTH = TOPIC_HEAD "/Config/" TOPIC_TOCU  "/Length";
//#define TOPIC_PREFIX "Arduino_"

/*
#define TOPIC_SUBSCRIPTION_PREFIX TOPIC_PREFIX"/ToCU"
#define TOPIC_PUBLISH_PREFIX TOPIC_PREFIX"/ToServer"
#define TOPIC_SUBSCRIPTION_BUTTONS TOPIC_SUBSCRIPTION_PREFIX"/Buttons"
#define TOPIC_PUBLISH_BUTTONS TOPIC_PUBLISH_PREFIX"/Buttons"
#define TOPIC_SUBSCRIPTION_RELAYS TOPIC_SUBSCRIPTION_PREFIX"/Relays"
#define TOPIC_PUBLISH_RELAYS TOPIC_PUBLISH_PREFIX"/Relays"
#define TOPIC_SUBSCRIPTION_LIGHTS TOPIC_SUBSCRIPTION_PREFIX"/Lights"
#define TOPIC_PUBLISH_LIGHTS TOPIC_PUBLISH_PREFIX"/Lights"
*/



/*
void PublishButton(ButtonUnit unit);
void PublishRelay(RelayUnit unit);
void PublishLight(LightUnit unit);
*/

