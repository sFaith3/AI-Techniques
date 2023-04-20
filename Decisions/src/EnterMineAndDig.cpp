#include "EnterMineAndDig.h"



EnterMineAndDig::EnterMineAndDig()
{
}

void EnterMineAndDig::Enter(Agent * agent)
{
	agent->setPosition(Vector2D(SRC_WIDTH / 2.f, 1.f * SRC_HEIGHT / 6.f));

}

void EnterMineAndDig::Update(Agent * agent)
{
	if (agent->gold >= pocketSize) {

		agent->gold = pocketSize;
		agent->ChangeState(new VisitBankAndDepositGold());
	} 
    if (agent->thirst >= thirstMax) {

		agent->thirst = thirstMax;
		agent->ChangeState(new DrinkAtSaloon());
	}
	else {
		agent->gold += goldRate;
		agent->thirst += thirstRate;

		agent->PrintStats();
	}


	
	

}


void EnterMineAndDig::Exit(Agent * agent)
{
}

EnterMineAndDig::~EnterMineAndDig()
{
}
