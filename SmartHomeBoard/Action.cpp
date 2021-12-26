// 
// 
// 

#include "action.h"
#include "SigmaEEPROM.h"
#include"Configuration.h"

extern Configuration Config;


bool Action::Compare(const Action* a) {
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
	if (header != NULL) {
		Config.Log->append(header);
	}
	Config.Log->append(F("Id:")).append(Id);
	Config.Log->append(F(";event:")).append(event);
	Config.Log->append(F(";originId:")).append(originId);
	Config.Log->append(F(";originType:")).append((char)originType);
	Config.Log->append(F(";targetAction:")).append(targetAction);
	Config.Log->append(F(";targetId:")).append(targetId);
	Config.Log->append(F(";targetType:")).append((char)targetType);
	
	Config.Log->append(F(" @ "));
	Config.Log->Log(level);
}

void Action::FillFrom(const Action* a) {
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


	Id = SigmaEEPROM::Read16(addr); //0-1
	originId = SigmaEEPROM::Read16(addr + 2); //2-3
	originType = (UnitType)SigmaEEPROM::Read8(addr + 4); 
	event = SigmaEEPROM::Read8(addr + 5);
	targetId = SigmaEEPROM::Read16(addr + 6); //6-7
	targetAction = (ActionType)SigmaEEPROM::Read8(addr + 8);
	targetType = (UnitType)SigmaEEPROM::Read8(addr + 9);

}

void Action::WriteToEEPROM(uint16_t addr) {
	//bool res = true;

	SigmaEEPROM::Write16(addr, Id);
	SigmaEEPROM::Write16(addr + 2, originId);
	SigmaEEPROM::Write8(addr + 4, originType);
	SigmaEEPROM::Write8(addr + 5, event);
	SigmaEEPROM::Write16(addr + 6, targetId);
	SigmaEEPROM::Write8(addr + 8, targetAction);
	SigmaEEPROM::Write8(addr + 9, targetType);
}
