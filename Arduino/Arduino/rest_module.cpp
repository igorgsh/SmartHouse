#include "rest_module.h"
//#include "global.h"

#include "Arduino.h"

#include "ext_global.h"

void SendPost(char * id, double oldValue, double newValue) {
	char str[100];
	sprintf(str, "Send Post: Unit ""%s"" changed value %u -> %u", id, (int)oldValue, (int)newValue);
	Serial.println(str);
}

void SendLog(char * id, double oldValue, double newValue) {
	char str[100];

	sprintf(str, "Log Record: Unit ""%s"" changed value %u -> %u", id, (int)oldValue, (int)newValue);
	Serial.println(str);
}

void ProcessServerRequests() {
}
