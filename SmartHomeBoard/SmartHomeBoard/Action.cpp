// 
// 
// 

#include "action.h"
//#include "ext_global.h"



bool Action::compare(Action* a) {
	if (a == NULL) return false;
	Debug2("id:", (Id == a->Id));
	Debug2("event:", (event == a->event));
	Debug2("originId:", (originId == a->originId));
	Debug2("origintType:", (originType == a->originType));
	Debug2("targetAction:", (targetAction == a->targetAction));
	Debug2("Targetid:", (targetId == a->targetId));

	return (
		Id == a->Id &&
		event == a->event &&
		originId == a->originId &&
		originType == a->originType &&
		targetAction == a->targetAction &&
		targetId == a->targetId
		);
}

void Action::print(const char* header,Stream& stream) {
	if (header != NULL) {
		stream.print(header);
	}
	stream.print("Id:");
	stream.print(Id, HEX);
	stream.print(";event:");
	stream.print(event, HEX);
	stream.print(";originId:");
	stream.print(originId, HEX);
	stream.print(";originType:");
	stream.print(originType,HEX);
	stream.print(";targetAction:");
	stream.print(targetAction,HEX);
	stream.print(";targetId:");
	stream.print(targetId,HEX);
	stream.println(";;;");
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

