#include "initdata.h"
#include "ext_global.h"
#include <Ethernet.h>
#include "light.h"
#include "relay.h"

void InitPins() {

	// Initialize buttons and pull it up
	for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
		if (Buttons[i].Pin != 0) {
			pinMode(Buttons[i].Pin, INPUT);
			digitalWrite(Buttons[i].Pin, HIGH);
		}
	}

	// Initialize lights
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++) {
		if (Lights[i].Pin != 0) {
			pinMode(Lights[i].Pin, OUTPUT);
			LightSet(Lights[i].Id, Lights[i].status);

		}
	}

	// Initialize Relays
	for (int i = 0; i < NUMBER_OF_RELAYS; i++) {
		if (Relays[i].Pin != 0) {
			pinMode(Relays[i].Pin, OUTPUT);
			RelaySet(Relays[i].Id, Relays[i].status);

		}
	}


}

void InitializeServer() {
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	server.begin();
	Log_(D_INFO,"Server is at ");
	Log(D_INFO,Ethernet.localIP());
}


void InitializeLights() {

	int n = 0;

	strcpy(Lights[n].Id, "LgtG");
	Lights[n].Pin = 40;
	DefaultLightValue(&Lights[n]);

	n++;
	strcpy(Lights[n].Id, "LgtR");
	Lights[n].Pin = 22;
	DefaultLightValue(&Lights[n]);

	n++;
	strcpy(Lights[n].Id, "LgtY");
	Lights[n].Pin = 23;
	DefaultLightValue(&Lights[n]);

	n++;

	strcpy(Lights[n].Id, "LgtB");
	Lights[n].Pin = 24;
	DefaultLightValue(&Lights[n]);

	n++;

	strcpy(Lights[n].Id, "LgtW");
	Lights[n].Pin = 43;
	DefaultLightValue(&Lights[n]);

	n++;
	Lights[n].Id[0] = 0;
}

static int actionIndex = 0;


void InitializeActions() {

	strcpy(Actions[actionIndex].Id, "Act1");
	strcpy(Actions[actionIndex].originId, "Btn0");
	Actions[actionIndex].action = Action::ACT_LIGHT_SWITCH;
	strcpy(Actions[actionIndex].targetId, "LgtG");
	Actions[actionIndex].event = BTN_ON;

	actionIndex++;
	strcpy(Actions[actionIndex].Id, "Act2");
	strcpy(Actions[actionIndex].originId, "Btn0");
	Actions[actionIndex].action = Action::ACT_LIGHT_ON;
	strcpy(Actions[actionIndex].targetId, "LgtG");
	Actions[actionIndex].event = BTN_SHORT_LONG;

	actionIndex++;
	strcpy(Actions[actionIndex].Id, "Act3");
	strcpy(Actions[actionIndex].originId, "Btn0");
	Actions[actionIndex].action = Action::ACT_LIGHT_DIM;
	strcpy(Actions[actionIndex].targetId, "LgtR");
	Actions[actionIndex].event = BTN_SHORT_LONG;
	
	actionIndex++;
	strcpy(Actions[actionIndex].Id, "Act4");
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_ON;
	strcpy(Actions[actionIndex].targetId, "Rl01");
	Actions[actionIndex].event = BTN_ON;

	actionIndex++;
	strcpy(Actions[actionIndex].Id, "Act5");
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_ON;
	strcpy(Actions[actionIndex].targetId, "SSR1");
	Actions[actionIndex].event = BTN_ON;

	actionIndex++;
	strcpy(Actions[actionIndex].Id, "Act6");
	strcpy(Actions[actionIndex].originId, "Btn1");
	Actions[actionIndex].action = Action::ACT_RELAY_OFF;
	strcpy(Actions[actionIndex].targetId, "Rl01");
	Actions[actionIndex].event = BTN_OFF;

	actionIndex++;
	strcpy(Actions[actionIndex].Id, "Act7");
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
	Buttons[n].lhOn = LOW;
	DefaultButtonValue(&Buttons[n]);

	n++;
	strcpy(Buttons[n].Id, "Btn1");
	Buttons[n].Pin = 42;
	Buttons[n].lhOn = LOW;
	DefaultButtonValue(&Buttons[n]);

	n++;
	Buttons[n].Id[0] = 0;

}

void InitializeRelays() {

	int n = 0;

	strcpy(Relays[n].Id, "Rl01");
	Relays[n].Pin = 43;
	Relays[n].lhOn = LOW;
	DefaultRelayValue(&Relays[n]);

	n++;
	strcpy(Relays[n].Id, "SSR1");
	Relays[n].Pin = 44;
	Relays[n].lhOn = HIGH;
	DefaultRelayValue(&Relays[n]);

	n++;
	Relays[n].Id[0] = 0;

}

void InitializeData() {

	InitializeButtons();
	InitializeLights();
	InitializeRelays();
	InitializeActions();
}
