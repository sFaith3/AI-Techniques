#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>
#include <ctime>
#include "SDL_SimpleApp.h"
#include "Vector2D.h"
#include "utils.h"
#include "SteeringBehavior.h"
#include "Path.h"
#include "FSMState.h"


class Agent
{
	friend class SteeringBehavior;
	friend class FSMState;

private:


	SteeringBehavior *steering_behavior;
	Vector2D position;
	Vector2D velocity;
	Vector2D target;

	float refreshRate = 200;
	float lastTime = 0;

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
	FSMState *currState;
	float gold = 0.0f;
	float money = 0.0f;
	float thirst = 0.0f;


	Agent();
	~Agent();
	void addPathPoint(Vector2D point);
	int getCurrentTargetIndex();
	void setCurrentTargetIndex(int idx);
	SteeringBehavior *Behavior();
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
	bool Agent::loadSpriteTexture(char* filename, int num_frames=1);

	void ChangeState(FSMState* new_state);
	void PrintStats();
	
};
