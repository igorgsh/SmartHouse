#pragma once
#include "peripheral.h"
#include "definitions.h"

  ButtonUnit Buttons[NUMBER_OF_BUTTONS];
  LightUnit Lights[NUMBER_OF_BUTTONS];
  RelayUnit Relays[NUMBER_OF_RELAYS];

  char  buf[50];
  DebugLevel dLevel = D_DEBUG;

