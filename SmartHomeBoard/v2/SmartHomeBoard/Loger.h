#pragma once
#include <Arduino.h>
#include "definitions.h"

class Loger
{
public:
	static void Log(DebugLevel level, String message);
	static void Error(String message) {
		Log(D_ERROR, message);
	}
	static void Fatal(String message) {
		Log(D_FATAL, message);
	}
	static void Info(String message) {
		Log(D_INFO, message);
	}
	static void Warn(String message) {
		Log(D_WARN, message);
	}

};

