#pragma once
#include <iostream>
#include <string>
#include "Agent.h"

class Agent;

class FSMState
{
public:
	virtual void Enter(Agent* agent) = 0;
	virtual void Update(Agent* agent) = 0;
	virtual void Exit(Agent* agent) = 0;

	virtual std::string GetName() = 0;
};
