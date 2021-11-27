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

	//nByte = ceil(pinsNumber / 8);
	ChildStates = new bool[pinsNumber];

	for (int i = pinsNumber-1; i >=0; i--) {
		ChildStates[i] = false;
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

void ShiftRegisterOut::Set(byte parentPin, bool st)
{
	if (parentPin <= pinsNumber) {
		ChildStates[parentPin] = st;
		Out();
	}
}
/*
byte ShiftRegisterOut::SetBit(byte x, int n, bool v)
{
	byte m = v;
	m = (m<<n) &  ~masks[n];
	
	x = (x & masks[n]) | m;
	return x;
}
*/

void ShiftRegisterOut::Out()
{
	digitalWrite(LatchPin, LOW);
	/*
	for (int i = 0; i < nByte; i++) {

		shiftOut(DPin, ClockPin, LSBFIRST, States[i]);
	}
	*/
	for (int j = pinsNumber-1; j >=0 ; j--) {
//		Config.Log->append("SIR. pin=").append(j).append("; b =").append(ChildStates[j]).Debug();
		digitalWrite(DPin, ChildStates[j]);

		digitalWrite(ClockPin, HIGH);
		digitalWrite(ClockPin, LOW);
	}

	//"защелкиваем" регистр, тем самым устанавлива¤ значени¤ на выходах
	digitalWrite(LatchPin, HIGH);
}
