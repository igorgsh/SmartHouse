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
	str0+=String(Id, HEX);
	str0 += ";event:";
	str0 += String(event, HEX);
	str0 += ";originId:";
	str0 += String(originId, HEX);
	str0 += ";originType:";
	str0 += String(originType,HEX);
	str0 += ";targetAction:";
	str0 += String(targetAction,HEX);
	str0 += ";targetId:";
	str0 += String(targetId,HEX);
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

/*
ActionUnit* FindAction(const char* id) {
	ActionUnit *unit = NULL;

	for (int i = 0; i < NUMBER_OF_ACTIONS && &(Actions[i]) != NULL; i++) {
		if (strcmp(Actions[i].Id, id) == 0) {
			unit = &(Actions[i]);
			break;
		}
		//if (Actions[i].Id[0] == 0)
		//	break;
	}
	return unit;

}

*/
/*
void DefaultActionValue(const ActionUnit* action) {

}
*/

