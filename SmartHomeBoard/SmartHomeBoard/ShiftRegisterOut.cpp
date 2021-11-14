#include "ShiftRegisterOut.h"


void ShiftRegisterOut::UnitLoop() {
	//Nothing
}

void ShiftRegisterOut::InitUnit()
{
	pinMode(LatchPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(Pin, OUTPUT);

	nByte = ceil(pinsNumber / 8);
	States = new byte(nByte);
	for (int i = 0; i < nByte; i++) {
		States[i] = 0;
	}
	Out();
}

void ShiftRegisterOut::FinalInitUnit()
{
	//Nothing
}

void ShiftRegisterOut::Set(byte parentPin, bool status)
{
	if (parentPin <= pinsNumber) {
		int nState = pinsNumber / 8;
		int pNumber = pinsNumber % 8;

		SetBit(States[nState], pNumber, status);
		Out();
	}
}

void ShiftRegisterOut::SetBit(byte x, int n, bool v)
{
	byte m = ~masks[n];
	x = (x & masks[n]) | m;
}

void ShiftRegisterOut::Out()
{
	digitalWrite(LatchPin, LOW);
	for (int i = 0; i < nByte; i++) {
		shiftOut(Pin, ClockPin, LSBFIRST, States[i]);
	}
	//"защелкиваем" регистр, тем самым устанавлива¤ значени¤ на выходах
	digitalWrite(LatchPin, HIGH);
}
