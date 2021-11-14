#include "ShiftRegister.h"
#include "SigmaEEPROM.h"

bool ShiftRegister::Compare(const Unit* u)
{
	if (u == NULL) return false;
	if (u->Type != UnitType::SHIFTOUT && u->Type != UnitType::SHIFTIN) return false;
	ShiftRegister* tu = (ShiftRegister*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		LatchPin == tu->LatchPin &&
		ClockPin == tu->ClockPin &&
		pinsNumber == tu->pinsNumber
		);
	return res;
}

void ShiftRegister::ConfigField(const JsonObject& jsonList) {
	//, '{"id":207, "type":"S", "Pin":7, "latch":8, "clock":9, "PinsN":16}'
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("latch")) {
		LatchPin = jsonList["latch"];
	}
	if (jsonList.containsKey("clock")) {
		ClockPin = jsonList["clock"];
	}
	if (jsonList.containsKey("PinsN")) {
		pinsNumber = jsonList["PinsN"];
	}
}

void ShiftRegister::ReadFromEEPROM(uint16_t addr)
{
	Id = SigmaEEPROM::Read16(addr);
	Type = SigmaEEPROM::Read8(addr + 2);
	Pin = SigmaEEPROM::Read8(addr + 3);
	LatchPin = SigmaEEPROM::Read8(addr + 4);
	ClockPin = SigmaEEPROM::Read8(addr + 5); 
	pinsNumber = SigmaEEPROM::Read8(addr + 6);

}

void ShiftRegister::WriteToEEPROM(uint16_t addr) {

	SigmaEEPROM::Write16(addr, Id);
	SigmaEEPROM::Write8(addr + 2, Type);
	SigmaEEPROM::Write8(addr + 3, Pin);
	SigmaEEPROM::Write8(addr + 4, LatchPin);
	SigmaEEPROM::Write8(addr + 5 , ClockPin);
	SigmaEEPROM::Write8(addr + 6, pinsNumber);

}
