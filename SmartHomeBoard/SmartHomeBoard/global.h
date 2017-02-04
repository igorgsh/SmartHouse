#pragma once


#include "types.h"
#include "definitions.h"
#include "Unit.h"
#include "mqtt.h"

Configuration Config;
DebugLevel dLevel = D_DEBUG;

EthernetClient EthClient;
Mqtt MqttClient;

