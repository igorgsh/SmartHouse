#include "Sigma_Clock.h"
#include "ArduinoJson.h"


Sigma_Clock::Sigma_Clock(EthernetClass& eth, const char* timezone)
{
    tz = timezone;
}

bool Sigma_Clock::GetClock(TimeElements& tm, bool isInternet)
{
    bool res = false;
    if (isInternet) {
        if (readClock()) {
            res = parseJson(&tm);
        }
    }
    if (!res) {
        res = RTC.read(tm);
    }

	return res;
}

bool Sigma_Clock::SetClock(TimeElements tm)
{
    bool res = RTC.write(tm);
    if (!res) {
        if (RTC.chipPresent()) {
            Serial.println("The DS1307 is stopped.  Please run the SetTime");
            Serial.println("example to initialize the time and begin running.");
            Serial.println();
        }
        else {
            Serial.println("DS1307 read error!  Please check the circuitry.");
            Serial.println();
        }

    }
    return res;
}

bool Sigma_Clock::SetClock()
{   
    bool res = false;
    TimeElements tm;

    res = GetClock(tm, true);
    if (res) {
        res = SetClock(tm);
    }
    return res;
}

const char* Sigma_Clock::PrintClock(const TimeElements* tm)
{
    TimeElements t;
    if (tm == NULL) {
        GetClock(t, false);
    }
    else {
        t = *tm;
    }
    sprintf(buf, "%u-%02u-%02u %02u:%02u:%02u", t.Year + 1970, t.Month, t.Day, t.Hour, t.Minute, t.Second);

    return buf;
}

bool Sigma_Clock::readClock()
{
    int numbTry = 0;
    int len = 0;
    bool res = false;

    while (numbTry < 5 && len == 0) {
        numbTry++;

        if (client.connect(server, 80)) {
            Serial.print("connected to ");
            Serial.println(client.remoteIP());
            // Make a HTTP request:
            Serial.print("Response:");
            Serial.println(client.available());
            sprintf(buf, CALENDAR_SERVER_PATH, tz);
            client.println(buf);
            sprintf(buf, "Host: %s", server);
            client.println(buf);
            client.println("Connection: close");
            client.println();

            int nTry = 0;
            len = client.available();
            Serial.print("len =");
            Serial.println(len);
            while (nTry <= 5 && len == 0) {
                delay(2000);
                nTry++;
                len = client.available();
                Serial.print(nTry);
                Serial.print("len =");
                Serial.println(len);
            }
        }
    }
    if (len == 0) {
        Serial.println("The len is zero");
        return res;
    }
    res =parseResponse(len);
    //Serial.println(buf);
	return res;
}

bool Sigma_Clock::parseResponse(int len)
{
    bool res = false;

    String s;
    //char eol[2];
    int payloadLen = 0;
    //int countBytes = 0;

    s = client.readString();

    int i = s.indexOf(HTTP_CONTENT_LENGTH);
    Serial.println(i);
    if (i != 0) {
        i += strlen(HTTP_CONTENT_LENGTH);
    }
    Serial.println(i);

    Serial.println(s.substring(i  + 1));
    Serial.println("#");
    payloadLen = s.substring(i + 1).toInt();
    Serial.println(payloadLen);
    if (payloadLen >= BUF_SIZE) {
        Serial.println("The size is too high");
        buf[0] = 0;
    }
    else if (payloadLen!=0) {       
        strcpy(buf, s.substring(s.length() - payloadLen).c_str());
    }
    else {
        buf[0] = 0;
    }
    res = (buf[0] != 0);
    return res;
}

bool Sigma_Clock::parseJson(TimeElements* t)
{
    bool res = false;
    if (buf[0] != 0) {
        const size_t CAPACITY = JSON_OBJECT_SIZE(BUF_SIZE);
        StaticJsonDocument<CAPACITY> doc;
        deserializeJson(doc, buf);
        // extract the data
        JsonObject root = doc.as<JsonObject>();
        
        if (root.containsKey("datetime")) {
            
            String s = root["datetime"]; //"2022-01-09T15:32:39.409582+02:00"

            t->Year = s.substring(0, 4).toInt()-1970;
            t->Month = s.substring(5, 7).toInt();
            t->Day = s.substring(8, 10).toInt();
            t->Hour = s.substring(11, 13).toInt();
            t->Minute = s.substring(14, 16).toInt();
            t->Second = s.substring(17, 19).toInt();
            res = true;
        }
    }
    return res;
}
