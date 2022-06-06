#include "SceneWander.h"

using namespace std;

SceneWander::SceneWander()
{
	Agent *agent = new Agent;
	agent->setPosition(Vector2D(640, 360));
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setRadius(100);
	agents.push_back(agent);
}

SceneWander::~SceneWander()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneWander::update(float dtime, SDL_Event *event)
{
	Vector2D steering_force = agents[0]->Behavior()->Wander(agents[0], 3.f,dtime, true);
	agents[0]->update(steering_force, dtime, event);
}

void SceneWander::draw()
{
	draw_circle(TheApp::Instance()->getRenderer(), (int)(agents[0]->getTarget().x), (int)(agents[0]->getTarget().y), 15, 255, 0, 0, 255);
	agents[0]->draw();
}

const char* SceneWander::getTitle()
{
	return "SDL Steering Behaviors :: Wander Demo";
}