#pragma once
#include <Arduino.h>
#include "definitions.h"


class Loger
{
public:

	static String LogMessage;

	static void Log(DebugLevel level) { Log(level, LogMessage); LogMessage = ""; }

	static void Log(DebugLevel level, const String &message);
	static void Error(const String &message) {
		Log(D_ERROR, message);
	}
	static void Error() {
		Log(D_ERROR);
	}
	static void Fatal(const String &message) {
		Log(D_FATAL, message);
	}
	static void Fatal() {
		Log(D_FATAL);
	}
	static void Info(const String &message) {
		Log(D_INFO, message);
	}
	static void Info() {
		Log(D_INFO);
	}
	static void Warn(const String &message) {
		Log(D_WARN, message);
	}
	static void Warn() {
		Log(D_WARN);
	}
	static void Debug(const String &message) {
		Log(D_DEBUG, message);
	}
	static void Debug() {
		Log(D_DEBUG);
	}
};

