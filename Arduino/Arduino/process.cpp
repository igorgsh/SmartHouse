#include "process.h"
#include "light.h"
#include "ext_global.h"
#include "relay.h"


void ProcessAction(char * id,  byte event, unsigned long newValue, unsigned long oldValue) {

	char buf[50];

	//sprintf(buf, "PAction: unit=%s, event=%u, newValue=%lu, oldValue=%lu", id, event, newValue, oldValue);
	//Debug(buf);


	for (int i = 0; i < NUMBER_OF_ACTIONS && &(Actions[i]) != NULL /*&& Actions[i].action != ACT_NO_ACTION*/; i++) {
		if (Actions[i].Id[0] != 0) {
			if (strcmp(Actions[i].originId, id) == 0) {
				if (Actions[i].event == event) {
					sprintf(buf, "ActionN: %u, event=%u", i, event);
					Debug(buf);
					switch (Actions[i].action)
					{
					case ACT_NO_ACTION: {
						break;
					}
					case ACT_LIGHT_SWITCH: {
						LightSwitch(Actions[i].targetId);
						break;
					}
					case ACT_LIGHT_DIM: {
						LightDim(Actions[i].targetId);
						break;
					}
					case ACT_LIGHT_ON: {
						LightSet(Actions[i].targetId, HIGH);
						break;
					}
					case ACT_LIGHT_OFF: {
						LightSet(Actions[i].targetId, LOW);
						break;
					}
					case ACT_RELAY_SWITCH: {
						RelaySwitch(Actions[i].targetId);
						break;
					}
					case ACT_RELAY_ON: {
						RelaySet(Actions[i].targetId, HIGH);
						break;
					}
					case ACT_RELAY_OFF: {
						RelaySet(Actions[i].targetId, LOW);
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
	}
}
