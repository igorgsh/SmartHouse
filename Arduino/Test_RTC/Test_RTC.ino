/*
 Name:		Test_RTC.ino
 Created:	02.02.17 10:38:31
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <RTClib.h>
#include <Wire.h>

DS3231 rtc;

void setup() {
	Serial.begin(115200);
	Wire.begin();
	rtc.begin();

	Serial.print("Now=");
	//Serial.println(rtc.now().format("DD.MM.YY - hh:mm:ss"));
	//Serial.println(rtc.now().format("DD.MM.YY - hh:mm:ss"));
	//if (!rtc.isrunning()) {
//		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		Serial.print("Date=");
		Serial.print(__DATE__);
		Serial.print(" Time=");
		Serial.println(__TIME__);
		DateTime dt = DateTime(__DATE__, __TIME__);
		//dt.setyear(2017);
		rtc.adjust(dt);
		//rtc.adjust(DateTime(__DATE__, __TIME__));

	//}
	//	Serial.print("Now2=");
		//Serial.println(rtc.now().format("DD.MM.YY - hh:mm:ss"));
		//rtc.write()
}

// the loop function runs over and over again until power down or reset
void loop() {
	DateTime now = rtc.now();

	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();

	Serial.print(" since midnight 1/1/1970 = ");
	Serial.print(now.unixtime());
	Serial.print("s = ");
	Serial.print(now.unixtime() / 86400L);
	Serial.println("d");

	// calculate a date which is 7 days and 30 seconds into the future
	DateTime future(now.unixtime() + 7 * 86400L + 30);

	Serial.print(" now + 7d + 30s: ");
	Serial.print(future.year(), DEC);
	Serial.print('/');
	Serial.print(future.month(), DEC);
	Serial.print('/');
	Serial.print(future.day(), DEC);
	Serial.print(' ');
	Serial.print(future.hour(), DEC);
	Serial.print(':');
	Serial.print(future.minute(), DEC);
	Serial.print(':');
	Serial.print(future.second(), DEC);
	Serial.println();

	Serial.println();
	delay(3000);
}
