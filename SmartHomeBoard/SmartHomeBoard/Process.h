#pragma once
#include <Arduino.h>
#include "types.h"
#include "definitions.h"

void ProcessAction(byte id,  byte event, unsigned long newValue, unsigned long oldValue);

