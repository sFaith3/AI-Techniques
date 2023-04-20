#pragma once
#include "FSMState.h"
#include "EnterMineAndDig.h"
#include "GoHomeAndRest.h"
#include "DrinkAtSaloon.h"


class VisitBankAndDepositGold :
	public FSMState
{
public:
	VisitBankAndDepositGold();

	void Enter(Agent* agent);
	void Update(Agent* agent);
	virtual std::string GetName() { return "BANK"; };
	void Exit(Agent* agent) ;
	~VisitBankAndDepositGold();
private:

	float maxMoney = 1000.0f;
	float goldRate = 0.4f;
	float moneyRate = 2.0f;
};

