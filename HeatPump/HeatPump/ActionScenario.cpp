#include "ActionScenario.h"



ActionScenario::ActionScenario()
{
}


ActionScenario::ActionScenario(int maxStep)
{
	this->maxStep = maxStep;
}

ActionScenario::~ActionScenario()
{
}

void ActionScenario::Run()
{
	currenStep = 0;
	NextStep();
	return;
}

void ActionScenario::NextStep() {

}
