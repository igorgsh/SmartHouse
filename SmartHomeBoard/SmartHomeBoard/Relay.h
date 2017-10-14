// relay.h
#pragma once
#include <Arduino.h>
#include "Unit.h"

class Relay : public Unit
{
public:
	void SetDefault();
	void InitUnit();
	void ProcessUnit(ActionType event);
	void UnitLoop();
	void FinalInitUnit() {};
private:
	void RelaySet(bool highLow);
	void RelaySwitch();
	void RelayOn() { RelaySet(lhOn); };
	void RelayOff() { RelaySet(!lhOn); };

};




