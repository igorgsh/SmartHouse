// utils.h
#pragma once
#include "Arduino.h"
#include "IPAddress.h"

extern int memoryFree();
void PrintIP(IPAddress addr, String &dest);
