#pragma once
#include <Ethernet.h>
#include <PubSubClient.h>
#include "definitions.h"
#include "types.h"
#include "configuration.h"
//#include "mqtt.h"

/*
extern ButtonUnit Buttons[NUMBER_OF_BUTTONS];
extern  LightUnit Lights[NUMBER_OF_LIGHTS];
extern  RelayUnit Relays[NUMBER_OF_RELAYS];
extern ActionUnit Actions[NUMBER_OF_ACTIONS];
*/

//extern char  buf[50];
extern DebugLevel dLevel;
//extern byte BoardId;
extern Configuration Config;

//extern EthernetServer Server;
extern EthernetClient EthClient;
//extern Mqtt MqttClient;
