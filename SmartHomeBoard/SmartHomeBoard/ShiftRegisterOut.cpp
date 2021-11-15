#include "ShiftRegisterOut.h"
#include "Configuration.h"

extern Configuration Config;

void ShiftRegisterOut::UnitLoop() {
	//Nothing
}

void ShiftRegisterOut::InitUnit()
{
	pinMode(LatchPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(DPin, OUTPUT);

	nByte = ceil(pinsNumber / 8);
	States = new byte(nByte);
	//Config.Log->append("Init ShiftRegisterOut: pinsN=").append(pinsNumber).append("; nByte=").append(nByte).Debug();
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
		int nState = parentPin / 8;
		int pNumber = parentPin % 8;
		//Config.Log->append("Set: pin=").append(parentPin).append(";nState=").append(nState).append(";pnumber=").append(pNumber).Debug();
		States[nState]=SetBit(States[nState], pNumber, status);
		Out();
	}
}

byte ShiftRegisterOut::SetBit(byte x, int n, bool v)
{
	//Config.Log->append("SetBit: x=").append(x).append(" ;n=").append(n).append(" ;v=").append(v).Debug();
	byte m = v;
	m = (m<<n) &  ~masks[n];
	
	x = (x & masks[n]) | m;
	//Config.Log->append("SetBit Final: x=").append(x).Debug();
	return x;
}

void ShiftRegisterOut::Out()
{
	digitalWrite(LatchPin, LOW);
	for (int i = 0; i < nByte; i++) {
		//Config.Log->append("OUT1:").append(States[i]).Debug();
		shiftOut(DPin, ClockPin, LSBFIRST, States[i]);
		//Config.Log->append("OUT2:").append(States[i]).Debug();
	}
	//"защелкиваем" регистр, тем самым устанавлива¤ значени¤ на выходах
	digitalWrite(LatchPin, HIGH);
}
