#pragma once
#include <vector>
#include "Agent.h"
#include "Vector2D.h"

class Agent;

class SteeringBehavior
{
private:
	float factor;
	float interval;
	float nextTime;
	Vector2D centerWonder;

public:
	SteeringBehavior();
	~SteeringBehavior();

	// K_SEPARATION_FORCE > K_COHESION_FORCE > K_ALIGNMENT_FORCE
	const float NEIGHBOR_RADIUS      = 300.f;
	const float K_SEPARATION_FORCE   = 2.f;
	const float K_COHESION_FORCE     = 4.f;
	const float K_ALIGNMENT_FORCE    = 0.f;
	const float K_MAX_FLOCKING_FORCE = 30.f;
	const float K_MAX_FISHING_FORCE  = 100.f;

	const float FISH_RADIUS = 50.f;
	const float TARGET_RADIUS = 150.f;
	const float TARGET_FORCE_FISH = 5.0f;
	const float SEPARATION_FORCE_FISH = .5;

	Vector2D KinematicSeek(Agent *agent, Vector2D target, float dtime);
	Vector2D KinematicSeek(Agent *agent, Agent *target, float dtime);
	Vector2D KinematicFlee(Agent *agent, Vector2D target, float dtime);
	Vector2D KinematicFlee(Agent *agent, Agent *target, float dtime);

	Vector2D Seek(Agent *agent, Vector2D target, float dtime);
	Vector2D Seek(Agent *agent, Agent *target, float dtime);
	Vector2D Flee(Agent *agent, Vector2D target, float dtime);
	Vector2D Flee(Agent *agent, Agent *target, float dtime);

	Vector2D Arrive(Agent *agent, Vector2D target, float dtime);
	Vector2D Arrive(Agent *agent, Agent *target, float dtime);

	Vector2D Pursue(Agent *agent, Agent *target, float dtime);
	Vector2D Evade(Agent *agent, Agent *target, float dtime);

	Vector2D Wander(Agent *agent,float angle, float dtime, bool drawCircle = false);

	Vector2D Flocking(Agent *agent, std::vector<Agent*> agents,float dtime);
	Vector2D Fishing(Agent *agent, std::vector<Agent*> agents, std::vector<Agent*> rocks, Vector2D target, float dtime);
};
