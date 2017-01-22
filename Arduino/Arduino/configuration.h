// configuration.h
#pragma once
#include "types.h"

void UpdateConfiguration(const ButtonUnit* unit);
void UpdateConfiguration(const LightUnit* unit);
void UpdateConfiguration(const RelayUnit* unit);
void UpdateConfiguration(const ActionUnit* unit);

void DeleteConfiguration(const ButtonUnit* unit);
void DeleteConfiguration(const LightUnit* unit);
void DeleteConfiguration(const RelayUnit* unit);
void DeleteConfiguration(const ActionUnit* unit);
