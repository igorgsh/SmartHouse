#include "initdata.h"
#include "ext_global.h"


void InitializeLights() {

	int n = 0;

	strcpy(Lights[n].Id, "LgtG");
	Lights[n].Pin = 40;
	Lights[n].status = LOW;
	Lights[n].dimValue = 1;

	n++;

	strcpy(Lights[n].Id, "LgtR");
	Lights[n].Pin = 22;
	Lights[n].status = LOW;
	Lights[n].dimValue = 1;

	n++;
	strcpy(Lights[n].Id, "LgtY");
	Lights[n].Pin = 23;
	Lights[n].status = LOW;
	Lights[n].dimValue = 1;

	n++;

	strcpy(Lights[n].Id, "LgtB");
	Lights[n].Pin = 24;
	Lights[n].status = LOW;
	Lights[n].dimValue = 1;

	n++;

	strcpy(Lights[n].Id, "LgtW");
	Lights[n].Pin = 43;
	Lights[n].status = LOW;
	Lights[n].dimValue = 0;

	n++;
	Lights[n].Id[0] = 0;
}

static int actionIndex = 0;


void InitializeActions() {

	strcpy(Actions[actionIndex].originId, "Btn0");
	Actions[actionIndex].action = Action::ACT_LIGHT_SWITCH;
	strcpy(Actions[actionIndex].targetId, "LgtG");
	Actions[actionIndex].event = BTN_ON;

	actionIndex++;
	strcpy(Actions[actionIndex].originId, "Btn0");
	Actions[actionIndex].action = Action::ACT_LIGHT_ON;
	strcpy(Actions[actionIndex].targetId, "LgtG");
	Actions[actionIndex].event = BTN_SHORT_LONG;

	actionIndex++;
	strcpy(Actions[actionIndex].originId, "Btn0");
	Actions[actionIndex].action = Action::ACT_LIGHT_DIM;
	strcpy(Actions[actionIndex].targetId, "LgtR");
	Actions[actionIndex].event = BTN_SHORT_LONG;
	
	actionIndex++;
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_ON;
	strcpy(Actions[actionIndex].targetId, "Rl01");
	Actions[actionIndex].event = BTN_ON;

	actionIndex++;
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_ON;
	strcpy(Actions[actionIndex].targetId, "SSR1");
	Actions[actionIndex].event = BTN_ON;

	actionIndex++;
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_OFF;
	strcpy(Actions[actionIndex].targetId, "Rl01");
	Actions[actionIndex].event = BTN_OFF;

	actionIndex++;
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_OFF;
	strcpy(Actions[actionIndex].targetId, "SSR1");
	Actions[actionIndex].event = BTN_OFF;
	

	actionIndex++;
	Actions[actionIndex].action = Action::ACT_NO_ACTION;


}

void InitializeButtons() {

	int n = 0;

	strcpy(Buttons[n].Id, "Btn0");
	Buttons[n].Pin = 30;
	Buttons[n].isLongMode = false;

	n++;
	strcpy(Buttons[n].Id, "Btn1");
	Buttons[n].Pin = 42;
	Buttons[n].isLongMode = false;
	
	n++;
	Buttons[n].Id[0] = 0;

}

void InitializeRelays() {

	int n = 0;

	strcpy(Relays[n].Id, "Rl01");
	Relays[n].Pin = 43;
	Relays[n].status = LOW;
	Relays[n].lhOn = LOW;

	n++;
	strcpy(Relays[n].Id, "SSR1");
	Relays[n].Pin = 44;
	Relays[n].status = LOW;
	Relays[n].lhOn = HIGH;

	n++;
	Relays[n].Id[0] = 0;

}

void InitializeData() {

	InitializeButtons();
	InitializeLights();
	InitializeRelays();
	InitializeActions();
}
