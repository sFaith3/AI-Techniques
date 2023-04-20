#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include "Vector2D.h"
#include "utils.h"
#include "SteeringBehavior.h"
#include "Path.h"
#include "PathFinding.h"

class Agent
{
	friend class SteeringBehavior;
	friend class PathFinding;

private:
	SteeringBehavior *steering_behavior;
	PathFinding *path_finding;
	Vector2D position;
	Vector2D velocity;
	Vector2D target;

	Path path;
	int currentTargetIndex;

	float mass;
	float orientation;
	float max_force;
	float max_velocity;

	SDL_Color color;

	SDL_Texture *sprite_texture;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;

public:
	Agent();
	~Agent();
	void addPathPoint(Vector2D point);
	int getCurrentTargetIndex();
	void setCurrentTargetIndex(int idx);
	SteeringBehavior *Behavior();
	PathFinding *PathBehavior();
	Vector2D getPosition();
	Vector2D getTarget();
	Vector2D getVelocity();
	float getMaxVelocity();
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setMass(float mass);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void update(Vector2D steering_force, float dtime, SDL_Event *event);
	void draw();
	bool Agent::loadSpriteTexture(char* filename, int num_frames = 1);
	void pathToPos(Vector2D target, Graph *graph, ENUM_PATH_TYPE pathType, bool clearPath = true);
	void pathToPositions(std::vector<Vector2D> targets, Graph *graph, ENUM_PATH_TYPE pathType);
	void clearPath();
};
