#pragma once
#include <vector>
#include "Scene.h"
#include "Agent.h"


class SceneFlocking:
	public Scene
{
public:
	SceneFlocking();
	~SceneFlocking();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
	int numAgents;
private:
	std::vector<Agent*> agents;
	Vector2D target;
};
