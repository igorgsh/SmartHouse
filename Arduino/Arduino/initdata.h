#if !defined(__INITDATA__)
#define __INITDATA__


void InitializeLights() {

  int n = 0;

  strcpy(Lights[n].Id, "LgtG");
  Lights[n].Pin = 40;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;

  strcpy(Lights[n].Id, "LgtR");
  Lights[n].Pin = 22;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;
  strcpy(Lights[n].Id, "LgtY");
  Lights[n].Pin = 23;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;

  strcpy(Lights[n].Id, "LgtB");
  Lights[n].Pin = 24;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;

  strcpy(Lights[n].Id, "LgtW");
  Lights[n].Pin = 43;
  Lights[n].status = LOW;
  Lights[n].dimValue = 0;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

}

void InitializeButtons() {

  int n = 0;
  int j = 0;

  strcpy(Buttons[n].Id, "Btn0");
  Buttons[n].Pin = 30;
  Buttons[n].isLongMode = false;
  Buttons[n].Actions[j].action = Action::ACT_LIGHT_SWITCH;
  strcpy(Buttons[n].Actions[j].Id, "LgtG");
  Buttons[n].Actions[j].condition = BTN_ON;
  j++;
  Buttons[n].Actions[j].action = Action::ACT_LIGHT_ON;
  strcpy(Buttons[n].Actions[j].Id, "LgtG");
  Buttons[n].Actions[j].condition = BTN_SHORT_LONG;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_LIGHT_DIM;
  strcpy(Buttons[n].Actions[j].Id, "LgtR");
  Buttons[n].Actions[j].condition = BTN_LONG;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_NO_ACTION;
  Buttons[n].startPressing = 0;

  n++;
  j = 0;
  strcpy(Buttons[n].Id, "Btn1");
  Buttons[n].Pin = 42;
  Buttons[n].isLongMode = false;
  Buttons[n].Actions[j].action = Action::ACT_RELAY_ON;
  strcpy(Buttons[n].Actions[j].Id, "Rl01");
  Buttons[n].Actions[j].condition = BTN_ON;
  j++;
  Buttons[n].Actions[j].action = Action::ACT_RELAY_ON;
  strcpy(Buttons[n].Actions[j].Id, "SSR1");
  Buttons[n].Actions[j].condition = BTN_ON;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_RELAY_OFF;
  strcpy(Buttons[n].Actions[j].Id, "Rl01");
  Buttons[n].Actions[j].condition = BTN_LONG;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_RELAY_OFF;
  strcpy(Buttons[n].Actions[j].Id, "SSR1");
  Buttons[n].Actions[j].condition = BTN_LONG;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_NO_ACTION;
  Buttons[n].startPressing = 0;

}

void InitializeRelays() {

	int n = 0;
	int j = 0;

	strcpy(Relays[n].Id, "Rl01");
	Relays[n].Pin = 43;
	Relays[n].status = LOW;
	Relays[n].lhOn = LOW;
	Relays[n].Actions[j].action = Action::ACT_NO_ACTION;

	n++;
	strcpy(Relays[n].Id, "SSR1");
	Relays[n].Pin = 44;
	Relays[n].status = LOW;
	Relays[n].lhOn = HIGH;
	Relays[n].Actions[j].action = Action::ACT_NO_ACTION;

}

void InitializeData() {

  InitializeButtons();
  InitializeLights();
  InitializeRelays();
}

#endif
