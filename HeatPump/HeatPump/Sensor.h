#pragma once
#include "Arduino.h"
#include "Relay.h"

typedef enum {
	NO_ERROR=0,
	SENSOR_DISCONNECTED=1,
	LOW_VALUE=2,
	HIGH_VALUE=3,
	CONTACT_ERROR = 4
} ErrorCode;
typedef enum {
	NOSENSOR = 0,
	THERMOMETER = 1,
	CONTACT = 2
} SensorType;

class Sensor
{
public:
	Sensor();
	Sensor(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, Relay* r, int critThreshold = 50);
	Sensor(String label, int pin, Relay* r, int critThreshold = 3);
	~Sensor();
	SensorType type = NOSENSOR;
	virtual float getValue() { return currentValue; };
	SensorType getType() { return type; };
	void setError(ErrorCode e) { error = e; };
	ErrorCode getError() { return error; };
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	int getPin() { return pin; };
	int ErrorCounter = 0;
	virtual bool isCritical() { return (ErrorCounter >= criticalThreshold); };
	virtual bool loop(unsigned long counter)=0;
	bool getData();
	bool isWaitingStart() { return waitingStart; };
	void setAlarmLow(float value) { alarmLow = value; };
	float getAlarmLow() { return alarmLow; };
	void setAlarmHigh(float value) { alarmHigh = value; };
	float getAlarmHigh() { return alarmHigh; };
	void setStartLow(float value) { startLow = value; };
	float getStartLow() { return startLow; };
	void setStartHigh(float value) { startHigh = value; };
	float getStartHigh() { return startHigh; };
	void setCriticalThreshold(int value) { criticalThreshold = value; };
	int getCriticalThreshold() { return criticalThreshold; };
	//static String ErrorToString(ErrorCode);
	const String ErrorStringShort[4] = { "","-","L","H" };
	const String ErrorStringLong[4] = { "","DISCONNECTED","LOW","HIGH" };
protected:
	virtual bool checkDataReady() = 0;
	ErrorCode error = NO_ERROR;
	String label;
	int pin;
	float alarmLow;
	float alarmHigh;
	float currentValue;
	float startLow;
	float startHigh;
	int criticalThreshold;
	int waitingStart = false;
	Relay* r;
private:
	void init(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, Relay* r, int critThreshold);

};
/*
class BasicSensor : public Sensor
{
public:
	bool loop(unsigned long counter) {};
private:
	bool checkDataReady() {
		return false;
	}
};
*/
