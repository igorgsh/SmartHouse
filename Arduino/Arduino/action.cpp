// 
// 
// 

#include "action.h"
#include "ext_global.h"

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

void DefaultActionValue(const ActionUnit* action) {

}
