#pragma once


#include "types.h"
#include "definitions.h"
#include "Unit.h"
#include "mqtt.h"

//Unit *Units;
Configuration Config;
DebugLevel dLevel = D_DEBUG;

/*
  ButtonUnit Buttons[NUMBER_OF_BUTTONS];
  LightUnit Lights[NUMBER_OF_LIGHTS];
  RelayUnit Relays[NUMBER_OF_RELAYS];
  ActionUnit Actions[NUMBER_OF_ACTIONS];
*/
  //char  buf[50];
  //const /*PROGMEM*/ byte MqttServer[] = { 192, 168, 0, 32 }; //IP-адрес сервера ioBroker
  //const /*PROGMEM*/ int MqttPort = 1883;

  //EthernetServer Server(ServerPort);
  EthernetClient EthClient;
  //PubSubClient *MqttClient;
  //void callback(char* topic, byte* payload, unsigned int length);
  //PubSubClient MqttClient(MqttServer, 1883, callback, EthClient);
  Mqtt MqttClient;

