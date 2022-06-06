#pragma once
#include "FSMState.h"
#include "GoHomeAndRest.h"
#include "DrinkAtSaloon.h"
#include "VisitBankAndDepositGold.h"

class EnterMineAndDig :
	public FSMState
{
public:
	EnterMineAndDig();

	void Enter(Agent* agent);
	void Update(Agent* agent);
	void Exit(Agent* agent);
	virtual std::string GetName() { return "MINE"; };
	~EnterMineAndDig();

private:
	float goldRate = 0.3f;
	float pocketSize = 100.0f;
	float thirstRate = 0.2f;
	float thirstMax = 100.0f;

};

