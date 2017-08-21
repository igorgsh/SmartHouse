// relay.h
#pragma once
#include <Arduino.h>
#include "Unit.h"

class Relay : public Unit
{
public:
	void SetDefault();
	void InitUnit();
	void RelaySet(bool highLow);
	void RelaySwitch();
	void ProcessUnit(int newStatus);
	void UnitLoop();
	void RelayOn() { RelaySet(lhOn); };
	void RelayOff() { RelaySet(!lhOn); };
	void FinalInitUnit() {};

};




