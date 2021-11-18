#include "ShiftRegisterIn.h"
#include "Configuration.h"

extern Configuration Config;


void ShiftRegisterIn::UnitLoop()
{
	//Config.Log->append("SIR:Unit loop Begin").Debug();
	digitalWrite(LatchPin, LOW);
	digitalWrite(LatchPin, HIGH);

	for (int i = 0; i < pinsNumber; i++) {
		bool b = digitalRead(DPin) != 0;
		if (children[i] != NULL) {
		//	Config.Log->append("SIR:Loop for id=").append(children[i]->Id).append(";b=").append(b).Debug();
			children[i]->ParentUnitLoop(b);
		}
		digitalWrite(ClockPin, HIGH);
		digitalWrite(ClockPin, HIGH);
	}
	//Config.Log->append("SIR:Unit loop End").Debug();
}

void ShiftRegisterIn::InitUnit()
{
	pinMode(LatchPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(DPin, INPUT);

	nByte = ceil(pinsNumber / 8);
	children = new Unit* [pinsNumber];
	//Config.Log->append("SIR:init nbyte=").append(nByte).append(" ;Parent=").append(parentId).Debug();
	

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
