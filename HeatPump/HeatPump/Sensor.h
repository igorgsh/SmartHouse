#pragma once
#include "Arduino.h"

typedef enum {
	NO_ERROR=0,
	SENSOR_DISCONNECTED=1,
	LOW_VALUE=2,
	HIGH_VALUE=3
} ErrorCode;

class Sensor
{
public:
	Sensor();
	Sensor(String label, int pin, float lowValue, float highValue);
	~Sensor();
	virtual float getValue() { return value; };
	virtual bool checkDataReady() =0;
	//virtual void setDataReady(bool val)=0;
	void setError(ErrorCode e) { error = e; };
	ErrorCode getError() { return error; };
	void setPrepareData(bool val) { preparation = val; };
	bool getPrepareData() { return preparation; };
	String getLabel() { return label; };
protected:
	ErrorCode error = NO_ERROR;
	String label;
	int pin;
	float lowValue;
	float highValue;
	float value;
	//bool isDataReady;
	bool preparation=false;
};

