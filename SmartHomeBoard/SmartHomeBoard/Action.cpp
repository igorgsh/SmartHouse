// 
// 
// 

#include "action.h"
#include "SigmaEEPROM.h"



bool Action::Compare(Action* a) {
	if (a == NULL) return false;

	return (
		Id == a->Id &&
		event == a->event &&
		originId == a->originId &&
		originType == a->originType &&
		targetAction == a->targetAction &&
		targetId == a->targetId &&
		targetType == a->targetType
		);
}

void Action::print(const char* header,DebugLevel level) {
	String str0 = "";

	if (header != NULL) {
		str0 = header;
	}
	str0+="Id:";
	str0+=String(Id, DEC);
	str0 += ";event:";
	str0 += String(event, DEC);
	str0 += ";originId:";
	str0 += String(originId, DEC);
	str0 += ";originType:";
	str0 += String((char)originType);
	str0 += ";targetAction:";
	str0 += String(targetAction,DEC);
	str0 += ";targetId:";
	str0 += String(targetId,DEC);
	str0 += ";targetType:";
	str0 += String((char)targetType);
	str0 += ";targetTypeHEX:";
	str0 += String(targetType,HEX);

	str0 += " @ ";
	Loger::Log(level, str0);
}

void Action::FillFrom(Action* a) {
	Id = a->Id;
	event = a->event;
	originId = a->originId;
	originType = a->originType;
	targetAction = a->targetAction;
	targetId = a->targetId;
	targetType = a->targetType;
}

void Action::InitAction() {

}


void Action::ReadFromEEPROM(uint16_t addr) {


	Id = SigmaEEPROM::Read8(addr);
	originId = SigmaEEPROM::Read8(addr + 1);
	originType = (UnitType)SigmaEEPROM::Read8(addr + 2);
	event = SigmaEEPROM::Read8(addr + 3);
	targetId = SigmaEEPROM::Read8(addr + 4);
	targetAction = (ActionType)SigmaEEPROM::Read8(addr + 5);
	targetType = (UnitType)SigmaEEPROM::Read8(addr + 6);

}

void Action::WriteToEEPROM(uint16_t addr) {
	//bool res = true;

	SigmaEEPROM::Write8(addr, Id);
	SigmaEEPROM::Write8(addr + 1, originId);
	SigmaEEPROM::Write8(addr + 2, originType);
	SigmaEEPROM::Write8(addr + 3, event);
	SigmaEEPROM::Write8(addr + 4, targetId);
	SigmaEEPROM::Write8(addr + 5, targetAction);
	SigmaEEPROM::Write8(addr + 6, targetType);

}
