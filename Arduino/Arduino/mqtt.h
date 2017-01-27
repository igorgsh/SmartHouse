// mqtt.h

#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "types.h"


#define TOPIC_PREFIX "Arduino_1"
#define TOPIC_SUBSCRIPTION_PREFIX TOPIC_PREFIX"/ToCU"
#define TOPIC_PUBLISH_PREFIX TOPIC_PREFIX"/ToServer"
#define TOPIC_SUBSCRIPTION_BUTTONS TOPIC_SUBSCRIPTION_PREFIX"/Buttons"
#define TOPIC_PUBLISH_BUTTONS TOPIC_PUBLISH_PREFIX"/Buttons"
#define TOPIC_SUBSCRIPTION_RELAYS TOPIC_SUBSCRIPTION_PREFIX"/Relays"
#define TOPIC_PUBLISH_RELAYS TOPIC_PUBLISH_PREFIX"/Relays"
#define TOPIC_SUBSCRIPTION_LIGHTS TOPIC_SUBSCRIPTION_PREFIX"/Lights"
#define TOPIC_PUBLISH_LIGHTS TOPIC_PUBLISH_PREFIX"/Lights"



//void callback(char* topic, byte* payload, unsigned int length);
void InitMqtt(void);
void MqttLoop();

void PublishButton(ButtonUnit unit);
void PublishRelay(RelayUnit unit);
void PublishLight(LightUnit unit);

#endif

