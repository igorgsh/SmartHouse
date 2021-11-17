#include "ShiftRegisterIn.h"
#include "Configuration.h"

extern Configuration Config;


void ShiftRegisterIn::UnitLoop()
{
	digitalWrite(LatchPin, LOW);
	digitalWrite(LatchPin, HIGH);

	for (int i = 0; i < pinsNumber; i++) {
		bool b = digitalRead(DPin) != 0;
		if (children[i] != NULL) {
			children[i]->ParentUnitLoop(b);
		}
		digitalWrite(ClockPin, HIGH);
		digitalWrite(ClockPin, HIGH);
	}
}

void ShiftRegisterIn::InitUnit()
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

void ShiftRegisterIn::FinalInitUnit()
{
	for (int i = 0; i < Config.numberUnits; i++) {
		if (Config.units[i]->parentId == Id) {
			children[Config.units[i]->Pin] = Config.units[i];
		}
	}
}
