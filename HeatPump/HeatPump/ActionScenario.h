#pragma once
#include "Scenario.h"

class ActionScenario :
	public Scenario
{
public:
	ActionScenario(int maxStep);
	~ActionScenario();
	void Run();
	void NextStep();
protected:
	Scenario* steps;
	int currenStep = 0;
	int maxStep;
};

