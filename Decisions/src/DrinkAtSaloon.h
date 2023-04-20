#pragma once
#include "FSMState.h"
#include "EnterMineAndDig.h"
#include "GoHomeAndRest.h"
#include "VisitBankAndDepositGold.h"

class DrinkAtSaloon :
	public FSMState
{
public:
	DrinkAtSaloon();

	void Enter(Agent* agent);
	void Update(Agent* agent);
	void Exit(Agent* agent);
	virtual std::string GetName() { return "SALOON"; };
	~DrinkAtSaloon();

public:
	float drinkingRate = 0.3;

};

