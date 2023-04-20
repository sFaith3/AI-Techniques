#pragma once
#include <vector>
#include "Agent.h"
#include "PathFinding.h"

class Agent;

class SteeringBehavior
{
public:
	SteeringBehavior();
	~SteeringBehavior();

	Path GetPath(Agent *agent, Graph *graph, Node *start, Node *goal, ENUM_PATH_TYPE pathType);
	Path GetPath(Agent *agent, Graph *graph, const Node *start, std::vector<Vector2D> goals);
	Vector2D Seek(Agent *agent, Vector2D target, float dtime);
	Vector2D Seek(Agent *agent, Agent *target, float dtime);
	Vector2D Arrive(Agent *agent, Vector2D target, int slow_radius, float dtime);
	Vector2D Arrive(Agent *agent, Agent *target, int slow_radius, float dtime);
	Vector2D SimplePathFollowing(Agent *agent, float dtime, bool clearPath = true);
	bool IsOtherAgentClose(Agent *agent, Agent *other, float dist);
};