#pragma once
#include "FSMState.h"
#include "EnterMineAndDig.h"
#include "DrinkAtSaloon.h"
#include "VisitBankAndDepositGold.h"

class GoHomeAndRest :
	public FSMState
{
public:
	GoHomeAndRest();

	void Enter(Agent* agent);
	void Update(Agent* agent);
	void Exit(Agent* agent);
	virtual std::string GetName() { return "HOME"; };
	~GoHomeAndRest();

private:
	float moneySave = 3.0f;
};

