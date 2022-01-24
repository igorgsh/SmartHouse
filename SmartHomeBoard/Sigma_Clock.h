#pragma once
#include <Ethernet.h>
#include "TimeLib.h"
#include "DS1307RTC.h"

#define CALENDAR_SERVER "worldtimeapi.org"
#define CALENDAR_SERVER_PATH "GET /api/timezone/%s HTTP/1.1"
#define BUF_SIZE	512
#define HTTP_CONTENT_LENGTH "content-length:"

class Sigma_Clock
{
public:
	Sigma_Clock(EthernetClass& eth, const char* timezone = "Europe/Kiev");
	bool GetClock(TimeElements& tm, bool isInternet=false);
	bool SetClock(TimeElements tm);
	bool SetClock();
	const char* PrintClock(const TimeElements* tm=NULL );

private:
	EthernetClient client;
	bool readClock();
	bool parseResponse(int len);
	bool parseJson(TimeElements* tm);
	const char* server = CALENDAR_SERVER;
	const char* tz;
	char buf[BUF_SIZE];

};

