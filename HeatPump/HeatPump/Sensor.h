#pragma once
#include "Arduino.h"
#include "Relay.h"
#include "ActionScenario.h"

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

typedef enum {
	ACTION_LOW = 0,
	ACTION_NORMAL = 1,
	ACTION_HIGH = 2,
	ACTION_NODATA = 3
} ActionStatus;

#define ACTIONS_NUMBER 4

class Sensor
{
public:
	Sensor();
	Sensor(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, ActionScenario* scenario[]=NULL, int critThreshold = 5);
	//Sensor(String label, int pin, ActionScenario* scenario[] = NULL, int critThreshold = 5);
	~Sensor();
	SensorType type = NOSENSOR;
	SensorType getType() { return type; };

	virtual float getValue() { return currentValue; };

	void setError(ErrorCode e) { error = e; };
	ErrorCode getError() { return error; };

	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };

	int getPin() { return pin; };
	int ErrorCounter = 0;
	virtual bool isCritical() { return (ErrorCounter >= criticalThreshold); };
	virtual bool loop(unsigned long counter)=0;
	bool getData();
	//bool isWaitingStart() { return waitingStart; };
	ActionStatus getActionStatus(){ return actionStatus; };

	void setAlarmLow(float value) { alarmLow = value; };
	float getAlarmLow() { return alarmLow; };
	void setAlarmHigh(float value) { alarmHigh = value; };
	float getAlarmHigh() { return alarmHigh; };
	void setStartLow(float value) { actionLow = value; };
	float getStartLow() { return actionLow; };
	void setStartHigh(float value) { actionHigh = value; };
	float getStartHigh() { return actionHigh; };
	void setCriticalThreshold(int value) { criticalThreshold = value; };
	int getCriticalThreshold() { return criticalThreshold; };
	//static String ErrorToString(ErrorCode);
	const String ErrorStringShort[4] = { "","-","L","H" };
	const String ErrorStringLong[4] = { "","DISCONNECTED","LOW","HIGH" };
protected:
	// Is Data Ready for this Sensor
	virtual bool checkDataReady() = 0;
	ErrorCode error = NO_ERROR;
	String label;
	int pin;
	// Alarm line: AlarmLow: -... alarmLow); No Alarm: alarmLow...alarmHigh;AlarmHigh:alarmHigh...
	// Action line: ActionLow:-...actionLow; ActionNormal:actionLow...actionHigh; ActionHigh: actionHigh...+ 
	float alarmLow;
	float alarmHigh;
	float actionLow;
	float actionHigh;

	ActionScenario* scenario[ACTIONS_NUMBER];


	// how many times alarm should be set before it became critical
	int criticalThreshold;

	//int waitingStart = false;
	ActionStatus actionStatus = ActionStatus::ACTION_LOW;

	float currentValue;
	//Relay* r;
private:
	void init(String label, int pin, float alarmLow, float alarmHigh, float startLow, float startHigh, /*Relay* r,*/ int critThreshold, ActionScenario* scenario[]);

};
