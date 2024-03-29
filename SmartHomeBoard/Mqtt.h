// mqtt.h

#pragma once 

#include <SafeString.h>
#include <PubSubClient.h>
#include "definitions.h"
#include "Unit.h"
//#include "configuration.h"
//#include "ext_global.h"




#define MQTT_INITIAL_RETRY_DELAY	1000 //delay between INITIAL reconnection retry
#define MQTT_RETRY_TIME				10000 //delay between mqtt reconnect in loop
#define MQTT_TOPIC_LENGTH	100
#define MQTT_PAYLOAD_LENGTH	200
#define MQTT_TRY_COUNT 5
#define MQTT_WAITING_RESPONSE 10000
#define MQTT_RESUBSCRIBE_TRY_COUNT 1
#define MQTT_RESUBSCRIPTION_DELAY	50
#define MQTT_ETHERNET_TIME (5*1000)


#define BOARD_ID "Board_%02d"
#define MQTT_SEPARATOR "/"

#define CHECK_MQTT	"info"

#define MQTT_CONFIG_COMMON "Config" MQTT_SEPARATOR "Common"


#define MQTT_WATCH_DOG "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "WatchDog"
#define MQTT_WATCH_DOG2 "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "WatchDog2"
#define MQTT_CONFIG_REQUEST "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "ConfigurationRequest"
#define MQTT_CONFIG_RESPONSE "Config" MQTT_SEPARATOR BOARD_ID MQTT_SEPARATOR "Configuration"
#define MQTT_ACTIONS_REQUEST "Config" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "ActionRequest"
#define MQTT_ACTIONS_RESPONSE "Config" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "Action"
#define MQTT_RESET_BOARD "Config" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "Reset"
#define MQTT_STATE_REQUEST MQTT_CONFIG_COMMON MQTT_SEPARATOR   "StateRequest"
#define MQTT_DELAY_OFF_6	MQTT_CONFIG_COMMON MQTT_SEPARATOR	"Delay_OFF_6"
#define MQTT_DELAY_ON_7		MQTT_CONFIG_COMMON MQTT_SEPARATOR	"Delay_ON_7"



#define MQTT_EQUIPMENT "Equipment" 
#define MQTT_VIRTUAL_EQUIPMENT MQTT_EQUIPMENT MQTT_SEPARATOR "Virtual"
#define MQTT_LOG  "Logs" MQTT_SEPARATOR BOARD_ID  MQTT_SEPARATOR "%s"
#define MQTT_BUTTONS MQTT_EQUIPMENT MQTT_SEPARATOR "Buttons"
#define MQTT_RELAYS  MQTT_EQUIPMENT MQTT_SEPARATOR "Relays"
#define MQTT_1WIREBUS  MQTT_EQUIPMENT MQTT_SEPARATOR "1-WireBuses"
#define MQTT_1WIRETHERMO  MQTT_EQUIPMENT MQTT_SEPARATOR "Thermo"
#define MQTT_POWERMETER  MQTT_EQUIPMENT MQTT_SEPARATOR "PowerMeter"
#define MQTT_VIRTUAL_BUTTONS  MQTT_VIRTUAL_EQUIPMENT MQTT_SEPARATOR "Buttons"
#define MQTT_CONTACTOR  MQTT_EQUIPMENT MQTT_SEPARATOR "Contactors"
#define MQTT_VOLTMETERDC  MQTT_EQUIPMENT MQTT_SEPARATOR "VoltmetersDC"


void callbackFunc(char* topic, uint8_t* payload, unsigned int length);

class Mqtt : public PubSubClient
{
	public:
		unsigned long mqttWaiting;
		void InitMqtt();
		void MqttLoop();
		void GetConfiguration();
		void GetActions();
		void PublishLog(DebugLevel level, const char* message);
		//void PublishUnit(const Unit* unit);
//		void SubscribeUnits();
//		void SubscribeUnit(int unitNumber);
		void Subscribe(const char* topic);
		bool Publish(const char* topic, const char* payload);
		//void Callback(const char* topic, const char* payload, unsigned int length);
		void Callback();

		Mqtt();
		void WatchDog();
		void WatchDog2();
		void PutBuffer(const char* topic, const char* payload, unsigned int len);
		char topicBuff[MQTT_TOPIC_LENGTH];
		char payloadBuff[MQTT_PAYLOAD_LENGTH];
		void RequestValue(const char* topic);

private:
		char *boardId = (char*)BOARD_ID;
		const char *LOG_END[7] = { "OFF", "FATAL","ERROR","WARN","INFO","DEBUG","ALL" };
		uint16_t GetUnitId(const char* str, int offset);
		//char topicBuff[MQTT_TOPIC_LENGTH];
		char topicLog[MQTT_TOPIC_LENGTH];
		//char topicLog[7][MQTT_TOPIC_LENGTH];
		bool MqttReconnect();
		char topicCB[MQTT_TOPIC_LENGTH];
		char payLoadCB[MQTT_PAYLOAD_LENGTH];
		unsigned int lenCB;
		void IdleLoop();
};

