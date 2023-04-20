#include "SceneFlocking.h"

using namespace std;

SceneFlocking::SceneFlocking()
{
	numAgents = 20;

	for (int i = 0; i < numAgents; i++) {
		Agent *agent = new Agent;
		agent->setPosition(Vector2D(rand() % (int)TheApp::Instance()->getWinSize().x, rand() % (int)TheApp::Instance()->getWinSize().y));
		agent->setTarget(Vector2D(640, 360));
		agent->loadSpriteTexture("res/soldier.png", 4);
		agents.push_back(agent);
		agent->setRadius(100);
		target = Vector2D(640, 360);
	}
}

SceneFlocking::~SceneFlocking()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneFlocking::update(float dtime, SDL_Event *event)
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
		break;
	default:
		break;
	}
	for (int i = 0; i < numAgents; i++) {
		Vector2D steering_force = agents[i]->Behavior()->Flocking(agents[i], agents ,dtime);
		agents[i]->update(steering_force, dtime, event);
	}
}

void SceneFlocking::draw()
{
	for (int i = 0; i < numAgents; i++) {
		agents[i]->draw();
	}	
}

const char* SceneFlocking::getTitle()
{
	return "Steering Behaviors :: Flocking Demo";
}