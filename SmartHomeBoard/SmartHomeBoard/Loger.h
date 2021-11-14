#pragma once
#include <Arduino.h>
#include "definitions.h"
#include <SafeString.h>


class Loger
{
public:
<<<<<<< HEAD

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
=======
	Loger& clear() { loger->clear(); return *this; };
	Loger& append(const char* str) { loger->concat(str);  return *this; };
	Loger& append(const SafeString& str) { loger->concat((SafeString&)str); return *this; };
	Loger& append(byte str) { loger->concat(str);  return *this; };
	Loger& append(uint16_t str) { loger->concat(str);  return *this; };
	Loger& append(int str) { loger->concat(str);  return *this; };
	Loger& append(long str) { loger->concat(str);  return *this; };
	Loger& append(double str) { loger->concat(str);  return *this; };
	Loger& append(const __FlashStringHelper* str) { loger->concat(str);  return *this; };

	Loger(int size);

	void Log(DebugLevel level);
	void Info(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_INFO); }
	void Info(const __FlashStringHelper* message) { loger->concat(message); Log(D_INFO); }

	void Warn(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_WARN); }
	void Debug(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_DEBUG); }
	void Error(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_ERROR); }
	void Fatal(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_FATAL); }
	void Warn(const __FlashStringHelper* message) { loger->concat(message);  Log(D_WARN); }
	void Debug(const __FlashStringHelper* message) { loger->concat(message);  Log(D_DEBUG); }
	void Error(const __FlashStringHelper* message) { loger->concat(message);  Log(D_ERROR); }
	void Fatal(const __FlashStringHelper* message) { loger->concat(message);  Log(D_FATAL); }


	char* loger_SAFEBUFFER;
	SafeString* loger;
private:
	int size;

>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
};

