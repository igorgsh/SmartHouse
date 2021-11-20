#include "ShiftRegisterOut.h"
#include "Configuration.h"

extern Configuration Config;

void ShiftRegisterOut::UnitLoop(unsigned long timePeriod, bool isParent, bool val) {
/*
	for (int i = 0; i < Config.numberUnits; i++) {
		if (Config.units[i]->parentId == Id) {
			Config.units[i]->UnitLoop(timePeriod,true, );
		}
	}
	*/
}

void ShiftRegisterOut::InitUnit(bool isParent)
{
	pinMode(LatchPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(DPin, OUTPUT);

	nByte = ceil(pinsNumber / 8);
	States = new byte(nByte);
	for (int i = 0; i < nByte; i++) {
		States[i] = 0;
	}
	Out();
	for (int i = 0; i < Config.numberUnits; i++) {
		if (Config.units[i]->parentId == Id) {
			Config.units[i]->InitUnit(true);
		}
	}
}

void ShiftRegisterOut::FinalInitUnit(bool isParent)
{
	for (int i = 0; i < Config.numberUnits; i++) {
		if (Config.units[i]->parentId == Id) {
			Config.units[i]->FinalInitUnit(true);
		}
	}
}

void ShiftRegisterOut::Set(byte parentPin, bool status)
{
	if (parentPin <= pinsNumber) {
		int nState = parentPin / 8;
		int pNumber = parentPin % 8;
		//Config.Log->append("Set: pin=").append(parentPin).append(";nState=").append(nState).append(";pnumber=").append(pNumber).Debug();
		States[nState]=SetBit(States[nState], pNumber, status);
		Out();
	}
}

byte ShiftRegisterOut::SetBit(byte x, int n, bool v)
{
	byte m = v;
	m = (m<<n) &  ~masks[n];
	
	x = (x & masks[n]) | m;
	return x;
}

void ShiftRegisterOut::Out()
{
	digitalWrite(LatchPin, LOW);
	for (int i = 0; i < nByte; i++) {
		shiftOut(DPin, ClockPin, LSBFIRST, States[i]);
	}
	//"защелкиваем" регистр, тем самым устанавлива¤ значени¤ на выходах
	digitalWrite(LatchPin, HIGH);
}
