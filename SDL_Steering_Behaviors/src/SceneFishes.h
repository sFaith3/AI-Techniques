#pragma once
#include <vector>
#include "Scene.h"
#include "Agent.h"


class SceneFishes:
	public Scene
{
public:
	SceneFishes();
	~SceneFishes();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
	int numAgents;
private:
	std::vector<Agent*> agents;
	std::vector<Agent*> rocks;
	Vector2D target;
};

