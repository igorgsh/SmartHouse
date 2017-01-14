#if !defined(__INITDATA__)
#define __INITDATA__


void InitializeLights() {

  int n = 0;

  strcpy(Lights[n].Id, "Lgt0");
  Lights[n].Pin = 40;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;

  strcpy(Lights[n].Id, "Lgt1");
  Lights[n].Pin = 22;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;
  strcpy(Lights[n].Id, "Lgt2");
  Lights[n].Pin = 23;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

  n++;

  strcpy(Lights[n].Id, "Lgt3");
  Lights[n].Pin = 24;
  Lights[n].status = LOW;
  Lights[n].dimValue = 1;
  Lights[n].Actions[0].action = Action::ACT_NO_ACTION;

}

void InitializeButtons() {

  int n = 0;
  int j = 0;

  strcpy(Buttons[n].Id, "Btn0");
  Buttons[n].Pin = 30;
  Buttons[n].isLongMode = false;
  Buttons[n].Actions[j].action = Action::ACT_LIGHT_SWITCH;
  strcpy(Buttons[n].Actions[j].Id, "Lgt0");
  Buttons[n].Actions[j].condition = BTN_ON;
  j++;
  Buttons[n].Actions[j].action = Action::ACT_LIGHT_ON;
  strcpy(Buttons[n].Actions[j].Id, "Lgt0");
  Buttons[n].Actions[j].condition = BTN_SHORT_LONG;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_LIGHT_DIM;
  strcpy(Buttons[n].Actions[j].Id, "Lgt1");
  Buttons[n].Actions[j].condition = BTN_LONG;

  j++;
  Buttons[n].Actions[j].action = Action::ACT_NO_ACTION;
  Buttons[n].startPressing = 0;

}

void InitializeData() {

  InitializeButtons();
  InitializeLights();
}

#endif
