#pragma once
#include "ScenarioItem.h"

class ActionScenario :
	public ScenarioItem
{
public:
	ActionScenario(int maxStep, ScenarioItem** items);
	~ActionScenario();
	bool NextStep();
	bool Run();
	bool IsRunning = false;
protected:
	ScenarioItem** steps;
	int currenStep = 0;
	int maxStep;
};

