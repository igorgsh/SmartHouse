#pragma once


#include "definitions.h"
#include "Unit.h"
#include "Mqtt.h"

Configuration Config;
DebugLevel dLevel = D_DEBUG;

EthernetClient EthClient;
Mqtt MqttClient;

