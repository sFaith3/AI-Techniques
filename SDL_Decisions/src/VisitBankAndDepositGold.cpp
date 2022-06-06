#include "VisitBankAndDepositGold.h"



VisitBankAndDepositGold::VisitBankAndDepositGold()
{
}

void VisitBankAndDepositGold::Enter(Agent * agent)
{
	agent->setPosition(Vector2D (SRC_WIDTH / 2.f, 3.f * SRC_HEIGHT / 4.f));
}

void VisitBankAndDepositGold::Update(Agent * agent)
{
	if (agent->gold <= 0.0f) {

		agent->gold = 0.0f;
		agent->ChangeState(new EnterMineAndDig());
	}
	if (agent->money >= 1000.0f) {

		agent->money = maxMoney;
		agent->ChangeState(new GoHomeAndRest());
	}
	else {
		agent->gold -= goldRate;
		agent->money += moneyRate;

		agent->PrintStats();
	}



}


void VisitBankAndDepositGold::Exit(Agent * agent)
{
}

VisitBankAndDepositGold::~VisitBankAndDepositGold()
{
}
