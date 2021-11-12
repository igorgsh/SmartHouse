// 
// 
// 

#include "action.h"
#include "SigmaEEPROM.h"
#include"ext_global.h"


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
		Log.append(header);
	}
	Log.append(F1("Id:")).append(Id);
	Log.append(F1(";event:")).append(event);
	Log.append(F1(";originId:")).append(originId);
	Log.append(F1(";originType:")).append((char)originType);
	Log.append(F1(";targetAction:")).append(targetAction);
	Log.append(F1(";targetId:")).append(targetId);
	Log.append(F1(";targetType:")).append((char)targetType);
	
	Log.append(F1(" @ "));
	Log.Log(level);
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
