#include "ActionScenario.h"
#include "SleepItem.h"


ActionScenario::ActionScenario(int maxStep, ScenarioItem** items)
{
	this->maxStep = maxStep;
	this->steps = items;
}

ActionScenario::~ActionScenario()
{
}

bool ActionScenario::NextStep()
{
	if (currenStep == maxStep) {
		IsRunning = false;
		return true;
	}
	currenStep++;
	return false;
}

bool ActionScenario::Run()
{
	if (currenStep <= maxStep) {
		IsRunning = true;
		if (dynamic_cast<SleepItem*> (steps[currenStep]) != NULL ) 
		steps[currenStep]->Run();
		currenStep++;
	}
	else {
		currenStep = 0;
		IsRunning = false;
	}
	return NextStep();
}
