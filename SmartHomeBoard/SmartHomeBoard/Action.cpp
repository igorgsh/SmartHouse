// 
// 
// 

#include "action.h"
//#include "ext_global.h"



bool Action::compare(Action* a) {
	if (a == NULL) return false;

	return (
		Id == a->Id &&
		event == a->event &&
		originId == a->originId &&
		originType == a->originType &&
		targetAction == a->targetAction &&
		targetId == a->targetId
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
}

void Action::InitAction() {

}
