#pragma once
#include <vector>
#include "Scene.h"
#include "Agent.h"


class SceneEvade:
	public Scene
{
public:
	SceneEvade();
	~SceneEvade();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	Vector2D target;
};
