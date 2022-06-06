#include "ScenePursue.h"

using namespace std;

ScenePursue::ScenePursue()
{
	Agent *agent = new Agent;
	agent->setPosition(Vector2D(300, 360));
	agent->setTarget(Vector2D(300, 360));
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);
	agent->setRadius(100);
	target = Vector2D(300, 360);

	Agent *enemy = new Agent;
	enemy->setPosition(Vector2D(900, 360));
	enemy->setMaxVelocity(150);
	enemy->setTarget(Vector2D(900, 360));
	enemy->loadSpriteTexture("../res/zombie1.png", 8);
	agents.push_back(enemy);
	enemy->setRadius(900);
	 
}

ScenePursue::~ScenePursue()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void ScenePursue::update(float dtime, SDL_Event *event)
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

	Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], agents[0]->getTarget(), dtime);
	agents[0]->update(steering_force, dtime, event); 
	agents[1]->setTarget(agents[0]->getPosition());
	steering_force = agents[1]->Behavior()->Pursue(agents[1], agents[0], dtime);
	agents[1]->update(steering_force, dtime, event);
}

void ScenePursue::draw()
{
	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);
	agents[0]->draw();
	agents[1]->draw();
}

const char* ScenePursue::getTitle()
{
	return "SDL Steering Behaviors :: Pursue Demo";
}