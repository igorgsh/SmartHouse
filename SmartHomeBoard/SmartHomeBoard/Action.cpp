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

<<<<<<< HEAD
void Action::print(const char* header, DebugLevel level) {

	String str0;
	str0.reserve(100);
	str0 = "";
=======
void Action::print(const char* header,DebugLevel level) {
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	if (header != NULL) {
		Config.Log->append(header);
	}
<<<<<<< HEAD
	str0 += F("Id:");
	str0 += Id;
	str0 += F(";event:");
	str0 += event;
	str0 += F(";originId:");
	str0 += originId;
	str0 += F(";originType:");
	str0 += ((char)originType);
	str0 += F(";targetAction:");
	str0 += targetAction;
	str0 += F(";targetId:");
	str0 += targetId;
	str0 += F(";targetType:");
	str0 += (char)targetType;

	str0 += F("@");
	Loger::Log(level, str0);
=======
	Config.Log->append(F1("Id:")).append(Id);
	Config.Log->append(F1(";event:")).append(event);
	Config.Log->append(F1(";originId:")).append(originId);
	Config.Log->append(F1(";originType:")).append((char)originType);
	Config.Log->append(F1(";targetAction:")).append(targetAction);
	Config.Log->append(F1(";targetId:")).append(targetId);
	Config.Log->append(F1(";targetType:")).append((char)targetType);
	
	Config.Log->append(F1(" @ "));
	Config.Log->Log(level);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
