#pragma once

#include "definitions.h"
#include "types.h"

void ProcessButton(ButtonUnit *unit);
ButtonUnit* FindButton(const char* id);
void DefaultButtonValue(ButtonUnit* btn);

