#pragma once

typedef enum {
	COMMAND_NO_COMMAND = 0,
	COMMAND_START = 1,
	COMMAND_STOP = 2,
	COMMAND_FORCE_STOP = 3
} ScenarioCommand;

class ScenarioItem
{
public:
	ScenarioItem();
	~ScenarioItem();
};

