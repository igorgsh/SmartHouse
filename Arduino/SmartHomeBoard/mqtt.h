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


void callbackFunc(char* topic, uint8_t* payload, unsigned int length);

class Mqtt : public PubSubClient
{
	public:

		void InitMqtt();
		void MqttLoop();
		void GetConfiguration();
		void PublishLog(DebugLevel level, String message);
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
		char *TOPIC_PREFIX = (char*)TOPIC_HEAD;
		char *TOPIC_CONFIG_REQUEST = (char*)(TOPIC_HEAD "/Config/" TOPIC_TOSERVER  "/Request");
		char *TOPIC_CONFIG_RESPONSE = (char*)(TOPIC_HEAD "/Config/" TOPIC_TOCU  "/Configuration");
		char *TOPIC_LOG = (char*)(TOPIC_HEAD "/Log");
		const char *LOG_SUFFIX[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };
		const int timeout = MQTT_RECONNECT_TIME;
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

