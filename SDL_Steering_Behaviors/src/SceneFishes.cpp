#include "SceneFishes.h"

using namespace std;

SceneFishes::SceneFishes()
{
	numAgents = 150;

	for (int i = 0; i < numAgents; i++) {
		Agent *agent = new Agent;
		agent->setPosition(Vector2D(rand() % (int)TheApp::Instance()->getWinSize().x, rand() % (int)TheApp::Instance()->getWinSize().y));
		agent->setTarget(Vector2D(640, 360));
		agent->loadSpriteTexture("../res/fish.png", 1);
		agents.push_back(agent);
		agent->setRadius(100);
		target = Vector2D(640, 360);
	}
}


SceneFishes::~SceneFishes()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneFishes::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			target = Vector2D((float)(event->button.x), (float)(event->button.y));
			agents[0]->setTarget(target);
		}
		if (event->button.button == SDL_BUTTON_RIGHT)
		{

			target = Vector2D((float)(event->button.x), (float)(event->button.y));
			Agent *agent = new Agent;
			agent->setPosition(target);
			agent->setAngle(RandomFloat(0,6.28f));
			agent->loadSpriteTexture("../res/Bigfish.png", 1);
			agent->setRadius(100);
			rocks.push_back(agent);
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < numAgents; i++) {
		Vector2D steering_force = agents[i]->Behavior()->Fishing(agents[i], agents, rocks, target, dtime);
		agents[i]->update(steering_force, dtime, event);
	}

	for (auto& i : rocks) {
		Vector2D steering_force = i->Behavior()->Wander(i, 30.f,dtime);
		i->update(steering_force, dtime, event);
	}
}

void SceneFishes::draw()
{
	for (int i = 0; i < numAgents; i++) {
		agents[i]->draw();
	}

	for (auto& i : rocks) {
		i->draw();
	}
}

const char* SceneFishes::getTitle()
{
	return "SDL Steering Behaviors :: Fishes";
}