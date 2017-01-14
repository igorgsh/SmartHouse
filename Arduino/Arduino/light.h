#pragma once
#include "peripheral.h"

LightUnit* FindLight(char* id);
void LightSet(char* id, bool isOn);

void LightSwitch(char* id);

void LightDim(char* Id);


