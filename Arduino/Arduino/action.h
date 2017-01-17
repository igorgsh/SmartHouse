// action.h
#pragma once

#include "definitions.h"
#include "types.h"

ActionUnit* FindAction(const char* id);
void DefaultActionValue(const ActionUnit* action);
