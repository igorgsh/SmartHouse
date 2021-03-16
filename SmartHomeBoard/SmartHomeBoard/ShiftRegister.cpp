#include "ShiftRegister.h"
#include "SigmaEEPROM.h"

void const ShiftRegister::print(const char* header, DebugLevel level)
{
	String str0 = "";

	if (header != NULL) {
		str0 = header;
	}
	str0 += F("Id:");
	str0 += (unsigned int)Id;
	str0 += F(";Type:");
	str0 += (char)Type;
	str0 += F(";Pin:");
	str0 += (unsigned int)Pin;
	str0 += F(";Latch:");
	str0 += (unsigned int)Latch;
	str0 += F(";Clock:");
	str0 += (unsigned int)Clock;
	str0 += F(";Steps:");
	str0 += (unsigned int)Steps;
	str0 += F(";status:");
	str0 += (unsigned int)status;
	str0 += F(";subscription:");
	str0 += (isSubscribed ? F("true") : F("false"));
	str0 += F(" @");
	Loger::Log(level, str0);

}

void ShiftRegister::ConfigField(const JsonObject& jsonList)
{
	if (jsonList.containsKey(F("Pin"))) {
		Pin = jsonList[F("Pin")];
	}
	if (jsonList.containsKey(F("latch"))) {
		Latch = jsonList[F("latch")];
	}
	if (jsonList.containsKey(F("clock"))) {
		Clock = jsonList[F("clock")];
	}
	if (jsonList.containsKey(F("steps"))) {
		Steps = jsonList[F("steps")];
	}
	if (jsonList.containsKey(F("status"))) {
		status = jsonList[F("status")];
	}
	print("ShiftRegister:", DebugLevel::D_DEBUG);
}



bool ShiftRegister::Compare(const Unit* u)
{
	if (u == NULL) return false;
	if (u->Type != UnitType::SHIFT_OUT && u->Type != UnitType::SHIFT_IN) return false;
	ShiftRegister* tu = (ShiftRegister*)u;
	bool res = (
		Id == tu->Id &&
		Type == tu->Type &&
		Pin == tu->Pin &&
		Latch == tu->Latch &&
		Clock == tu->Clock &&
		Steps == tu->Steps &&
		status == tu->status
		);
	return res;
}

void ShiftRegister::InitUnit()
{
	pinMode(Pin, OUTPUT);
}

void ShiftRegister::ReadFromEEPROM(uint16_t addr)
{
	Id = SigmaEEPROM::Read8(addr);
	Type = SigmaEEPROM::Read8(addr + 1);
	Pin = SigmaEEPROM::Read8(addr + 2);
	Latch = SigmaEEPROM::Read8(addr + 3);
	Clock = SigmaEEPROM::Read8(addr + 4);
	Steps = SigmaEEPROM::Read8(addr + 5);
	status = SigmaEEPROM::Read8(addr + 6);

}

void ShiftRegister::WriteToEEPROM(uint16_t addr)
{
	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, Type);
	SigmaEEPROM::Write8(addr + 2, Pin);
	SigmaEEPROM::Write8(addr + 3, Latch);
	SigmaEEPROM::Write8(addr + 4, Clock);
	SigmaEEPROM::Write8(addr + 5, Steps);
	SigmaEEPROM::Write8(addr + 4, status);

}

