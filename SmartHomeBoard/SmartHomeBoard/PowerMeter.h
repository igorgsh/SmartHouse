#pragma once
#include "Unit.h"
#include <PZEM004T.h>

class PowerMeter :
	public Unit
{
public:
	PowerMeter();
	~PowerMeter();
	void InitUnit();
	
	double voltage();
	double current();
	double power();
	double energy();

	void UnitLoop();
	void PublishAll();
	void SetDefault();
	void FinalInitUnit();
	void ProcessUnit(ActionType action);

	/*
	virtual void FillFrom(Unit* u);
	virtual void const print(const char* header, DebugLevel level);
	*/

private:
	HardwareSerial *port=NULL;
	PZEM004T* pzem=NULL;
	IPAddress ip = IPAddress(10, 10, 10, 10);
	
};

