#include "DrinkAtSaloon.h"



DrinkAtSaloon::DrinkAtSaloon()
{
}

void DrinkAtSaloon::Enter(Agent * agent)
{
	agent->setPosition(Vector2D(3.f * SRC_WIDTH / 4.f, 3.f * SRC_HEIGHT / 4.f));

}

void DrinkAtSaloon::Update(Agent * agent)
{
	if (agent->thirst <= 0.0f) {

		agent->thirst = 0.0f;
		agent->ChangeState(new EnterMineAndDig());
	}
	else {

		agent->thirst -= drinkingRate;

		agent->PrintStats();
	}
}


void DrinkAtSaloon::Exit(Agent * agent)
{
}

DrinkAtSaloon::~DrinkAtSaloon()
{
}
