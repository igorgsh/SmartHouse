#pragma once
#include <Ethernet.h>
#include <PubSubClient.h>
#include "definitions.h"
#include "types.h"

extern ButtonUnit Buttons[NUMBER_OF_BUTTONS];
extern  LightUnit Lights[NUMBER_OF_LIGHTS];
extern  RelayUnit Relays[NUMBER_OF_RELAYS];
extern ActionUnit Actions[NUMBER_OF_ACTIONS];


//extern char  buf[50];
extern DebugLevel dLevel;
extern byte mac[];
extern IPAddress ip;
extern const byte ServerPort;

//extern EthernetServer Server;
extern EthernetClient EthClient;
extern PubSubClient MqttClient;
