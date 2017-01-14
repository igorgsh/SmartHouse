#include "process.h"
#include "light.h"
#include "ext_global.h"
#include "relay.h"


void ProcessAction(char * id, ActionDescription * actions, byte event, unsigned long newValue, unsigned long oldValue) {

	//sprintf(buf, "ProcessAction: unit=%s, newValue=%lu, oldValue=%lu", id, newValue, oldValue);
	//Log(D_DEBUG, buf);

	char buf[50];

	for (int i = 0; i < NUMBER_OF_ACTIONS && &(actions[i]) != NULL && actions[i].action != ACT_NO_ACTION; i++) {
		//sprintf(buf, "ActionN: %u, event=%u, action=%u", i, event, actions[i].condition);
		//Log(D_DEBUG, buf);
		if (actions[i].condition == event) {
			switch (actions[i].action)
			{
			case ACT_NO_ACTION: {
				break;
			}
			case ACT_LIGHT_SWITCH: {
				LightSwitch(actions[i].Id);
				break;
			}
			case ACT_LIGHT_DIM: {
				LightDim(actions[i].Id);
				break;
			}
			case ACT_LIGHT_ON: {
				LightSet(actions[i].Id, HIGH);
				break;
			}
			case ACT_LIGHT_OFF: {
				LightSet(actions[i].Id, LOW);
				break;
			}
			case ACT_RELAY_SWITCH: {
				RelaySwitch(actions[i].Id);
				break;
			}
			case ACT_RELAY_ON: {
				RelaySet(actions[i].Id, HIGH);
				break;
			}
			case ACT_RELAY_OFF: {
				RelaySet(actions[i].Id, LOW);
				break;
			}

			case ACT_SEND_POST: {
				//SendPost(act.Id, unit.Value, newValue);
				break;
			}
			case ACT_SEND_LOG: {
				//SendLog(act.Id, unit.Value, newValue);
				break;
			}
			default:
				break;
			}
		}
	}
}
