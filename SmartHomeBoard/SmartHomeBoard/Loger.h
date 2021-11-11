#pragma once
#include <Arduino.h>
#include "definitions.h"
#include <SafeString.h>

class Loger
{
public:
	Loger& clear() { loger->clear(); return *this; };
	Loger& append(const SafeString& str) { loger->concat((SafeString&)str); return *this; };
	Loger& append(const char* str) { loger->concat(str); return *this; };
	Loger& append(byte str) { loger->concat(str); return *this; };
	Loger& append(uint16_t str) { loger->concat(str); return *this; };
	Loger& append(int str) { loger->concat(str); return *this; };
	Loger& append(long str) { loger->concat(str); return *this; };
	Loger& append(double str) { loger->concat(str); return *this; };
	Loger& append(const __FlashStringHelper* str) { loger->concat(str); return *this; };

	Loger(int size);

	void Log(DebugLevel level);
	void Info(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_INFO); }
	void Info(const __FlashStringHelper* message) { Log(D_INFO); }

	void Warn(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_WARN); }
	void Debug(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_DEBUG); }
	void Error(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_ERROR); }
	void Fatal(const char* message = NULL) { if (message != NULL) { loger->concat(message); } Log(D_FATAL); }
	void Warn(const __FlashStringHelper* message) { Log(D_WARN); }
	void Debug(const __FlashStringHelper* message) { Log(D_DEBUG); }
	void Error(const __FlashStringHelper* message) { Log(D_ERROR); }
	void Fatal(const __FlashStringHelper* message) { Log(D_FATAL); }


	char* loger_SAFEBUFFER;
	SafeString* loger;
private:
	int size;

};

