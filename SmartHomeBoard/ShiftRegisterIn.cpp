#include "ShiftRegisterIn.h"
#include "Configuration.h"

extern Configuration Config;


void ShiftRegisterIn::UnitLoop(unsigned long timePeriod, bool isParent, bool val)
{
	digitalWrite(LatchPin, LOW);
	digitalWrite(LatchPin, HIGH);
	for (int j = 0; j < nByte; j++) {
		for (int i = 7; i >= 0; i--) {
			bool b = (digitalRead(DPin) != 0);
			//Config.Log->append("SIR. pin=").append(j*8 + i).append("; b =").append(b).Debug();
			if (children[j * 8 + i] != NULL) {
				//Config.Log->append("Pass:(").append(j * 8 + i).append(") =").append(b).Debug();
				children[j * 8 + i]->UnitLoop(timePeriod, true, b);
			}
			digitalWrite(ClockPin, HIGH);
			digitalWrite(ClockPin, LOW);
		}
	}
}

void ShiftRegisterIn::InitUnit(bool isParent)
{
	pinMode(LatchPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(DPin, INPUT);

	nByte = ceil(pinsNumber / 8);
	children = new Unit* [pinsNumber];

	for (int i = 0; i < pinsNumber; i++) {
		children[i] = NULL;
	}
}

void ShiftRegisterIn::FinalInitUnit(bool isParent)
{
	for (int i = 0; i < Config.numberUnits; i++) {
		if (Config.units[i]->parentId == Id) {
			children[Config.units[i]->Pin] = Config.units[i];
			Config.units[i]->InitUnit(true);
		}
	}
}
