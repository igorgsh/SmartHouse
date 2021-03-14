#include "ShiftRegister.h"
#include "SigmaEEPROM.h"

void const ShiftRegister::print(const char* header, DebugLevel level)
{
	String str0 = "";

	if (header != NULL) {
		str0 = header;
	}
	str0 += "Id:";
	str0 += String((unsigned int)Id, DEC);
	str0 += ";Type:";
	str0 += String((char)Type);
	str0 += ";Pin:";
	str0 += String((unsigned int)Pin, DEC);
	str0 += ";Latch:";
	str0 += String((unsigned int)Latch, DEC);
	str0 += ";Clock:";
	str0 += String((unsigned int)Clock, DEC);
	str0 += ";Steps:";
	str0 += String((unsigned int)Steps, DEC);
	str0 += ";status:";
	str0 += String((unsigned int)status, DEC);
	str0 += ";subscription:";
	str0 += (isSubscribed ? "true" : "false");
	str0 += " @";
	Loger::Log(level, str0);

}

void ShiftRegister::ConfigField(JsonObject& jsonList)
{
	if (jsonList.containsKey("Pin")) {
		Pin = jsonList["Pin"];
	}
	if (jsonList.containsKey("latch")) {
		Latch = jsonList["latch"];
	}
	if (jsonList.containsKey("clock")) {
		Clock = jsonList["clock"];
	}
	if (jsonList.containsKey("steps")) {
		Steps = jsonList["steps"];
	}
	if (jsonList.containsKey("status")) {
		status = jsonList["status"];
	}
}



bool ShiftRegister::Compare(Unit* u)
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
	if (!res) {
		Loger::Debug("Compare Shift Register:" + String(Id == tu->Id) + ":" + String(Type == tu->Type) + ":" + String(Pin == tu->Pin) + ":"
			+ String(Latch == tu->Latch) + ":" + String(Clock == tu->Clock) + ":" + String(Steps == tu->Steps) + ":" + String(status == tu->status) + "#");
	}
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

