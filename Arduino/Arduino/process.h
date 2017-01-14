#pragma once
#include "peripheral.h"
#include "definitions.h"

void ProcessAction(char* id, ActionDescription* actions, byte event, unsigned long newValue, unsigned long oldValue);

