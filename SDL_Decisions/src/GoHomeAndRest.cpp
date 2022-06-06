#include "GoHomeAndRest.h"



GoHomeAndRest::GoHomeAndRest()
{
}

void GoHomeAndRest::Enter(Agent * agent)
{

	agent->setPosition(Vector2D(SRC_WIDTH / 4.f, 3.f * SRC_HEIGHT / 4.f));

}

void GoHomeAndRest::Update(Agent * agent)
{
	if (agent->money <= 0.0f) {

		agent->money = 0.0f;
		agent->ChangeState(new EnterMineAndDig());

	}
	else {

		agent->money -= moneySave;

		agent->PrintStats();

	}

	
	
	
}


void GoHomeAndRest::Exit(Agent * agent)
{
}

GoHomeAndRest::~GoHomeAndRest()
{
}
