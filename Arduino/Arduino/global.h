#pragma once

#include "types.h"
#include "definitions.h"

  ButtonUnit Buttons[NUMBER_OF_BUTTONS];
  LightUnit Lights[NUMBER_OF_LIGHTS];
  RelayUnit Relays[NUMBER_OF_RELAYS];
  ActionUnit Actions[NUMBER_OF_ACTIONS];

  //char  buf[50];
  DebugLevel dLevel = D_DEBUG;
  byte mac[] = {0x00, 0xAA, 0x22, 0x07, 0x19, 0x69  };
  // IP address in case DHCP fails
  IPAddress ip(192, 168, 0, 101);
  const PROGMEM byte ServerPort = 80;
  const /*PROGMEM*/ byte MqttServer[] = { 192, 168, 0, 32 }; //IP-адрес сервера ioBroker
  const /*PROGMEM*/ int MqttPort = 1883;

  //EthernetServer Server(ServerPort);
  EthernetClient EthClient;
  //PubSubClient *MqttClient;
  void callback(char* topic, byte* payload, unsigned int length);
  PubSubClient MqttClient(MqttServer, 1883, callback, EthClient);

