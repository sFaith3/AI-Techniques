#include "SteeringBehavior.h"



SteeringBehavior::SteeringBehavior()
{
	factor = 1.f;
	interval = 500.f;
	nextTime = 0.f;
	centerWonder = 0.f;
}


SteeringBehavior::~SteeringBehavior()
{
}

Vector2D SteeringBehavior::KinematicSeek(Agent *agent, Vector2D target, float dtime)
{
	Vector2D steering = target - agent->position;
	steering.Normalize();

	return steering * agent->max_velocity;
}

Vector2D SteeringBehavior::KinematicSeek(Agent *agent, Agent *target, float dtime)
{
	return KinematicSeek(agent, target->position, dtime);
}

Vector2D SteeringBehavior::KinematicFlee(Agent *agent, Vector2D target, float dtime)
{
	Vector2D steering = agent->position - target;
	steering.Normalize();

	return steering * agent->max_velocity;
}

Vector2D SteeringBehavior::KinematicFlee(Agent *agent, Agent *target, float dtime)
{
	return KinematicFlee(agent, target->position, dtime);
}

Vector2D SteeringBehavior::Seek(Agent *agent, Vector2D target, float dtime)
{
	Vector2D DesiredVelocity = target - agent->position;
	DesiredVelocity.Normalize();
	DesiredVelocity *= agent->max_velocity * factor;
	Vector2D SteeringForce = (DesiredVelocity - agent->velocity);
	if (factor == 0.f) {
		return Vector2D(0.f);
	}
	else {
		SteeringForce /= agent->max_velocity * factor;
	}

	return SteeringForce * agent->max_force;
}

Vector2D SteeringBehavior::Seek(Agent *agent, Agent *target, float dtime)
{
	return Seek(agent, target->position, dtime);
}

Vector2D SteeringBehavior::Flee(Agent *agent, Vector2D target, float dtime)
{
	Vector2D DesiredVelocity = agent->position - target;
	DesiredVelocity.Normalize();
	DesiredVelocity *= agent->max_velocity;
	Vector2D SteeringForce = (DesiredVelocity - agent->velocity);
	SteeringForce /= agent->max_velocity;

	return SteeringForce * agent->max_force;
}

Vector2D SteeringBehavior::Flee(Agent *agent, Agent *target, float dtime)
{
	return Flee(agent, target->position, dtime);
}

Vector2D SteeringBehavior::Arrive(Agent *agent, Vector2D target, float dtime)
{
	float distanceToTarget = Vector2D::Distance(agent->position, target);
	if (distanceToTarget > agent->radius) {
		factor = 1;
	}
	else {
		factor = distanceToTarget / agent->radius;
	}

	return Seek(agent, target, dtime);
}

Vector2D SteeringBehavior::Arrive(Agent *agent, Agent *target, float dtime)
{
	return Arrive(agent,target->position,dtime);
}

Vector2D SteeringBehavior::Pursue(Agent *agent, Agent *target, float dtime)
{
	float distanceToTarget = Vector2D::Distance(agent->position, target->position);
	if (target->velocity.Length() == 0.f) {
		return Seek(agent, target->position, dtime);
	}
	float T = distanceToTarget / target->velocity.Length();
	Vector2D predictedTarget = target->position + target->velocity * T;

	return Seek(agent, predictedTarget, dtime);
}

Vector2D SteeringBehavior::Evade(Agent *agent, Agent *target, float dtime)
{
	float distanceToTarget = Vector2D::Distance(agent->position, target->position);
	if (target->velocity.Length() == 0.f) {
		return Flee(agent, target->position, dtime);
	}
	float T = distanceToTarget / target->velocity.Length();
	Vector2D predictedTarget = target->position + target->velocity * T;

	return Flee(agent, predictedTarget, dtime);
}

Vector2D SteeringBehavior::Wander(Agent *agent, float angle, float dtime, bool drawCircle)
{
	float r = 100.f;
	float offset = 200.f;
	float maxAngle = ((RandomFloat(-angle, angle) * 3.14f) / 180.f);

	float tmp = RandomFloat(-maxAngle, maxAngle);
	agent->angle += tmp;

	centerWonder = agent->position + agent->getVelocity().Normalize() * offset;
	agent->target = Vector2D(centerWonder.x + (r * cos(agent->angle)), centerWonder.y + (r * sin(agent->angle)));

	if (drawCircle) {
		draw_circle(TheApp::Instance()->getRenderer(), (int)centerWonder.x, (int)centerWonder.y, r, 255, 0, 0, 255);
	}
	return Seek(agent, agent->target, dtime);

}


Vector2D SteeringBehavior::Flocking(Agent *agent,std::vector<Agent*> agents, float dtime)
{
	Vector2D separationDirection, cohesionDirection, alignmentDirection, flockingForce;
	
	///Separation;
	Vector2D separationVector = Vector2D(0.f);
	int neighborCount = 0;
	for (int i = 0; i < (int)agents.size(); i++) {

		if (Vector2D::Distance(agent->position, agents[i]->position) < NEIGHBOR_RADIUS)
		{
			separationVector += agent->position - agents[i]->position;
			neighborCount++;
		}


	}

	separationVector /= neighborCount;
	separationDirection = Vector2D::Normalize(separationVector);

	///Cohesion
	neighborCount = 0;
	Vector2D averagePosition = Vector2D(0.f);

	for (auto& currAgent : agents)
	{
		if (Vector2D::Distance(currAgent->position, agent->position) < NEIGHBOR_RADIUS)
		{
			averagePosition += currAgent->position;
			neighborCount++;
		}
	}

	averagePosition /= neighborCount;
	averagePosition -= agent->position;
	cohesionDirection = Vector2D::Normalize(averagePosition);

	///Alignment
	neighborCount = 0;
	Vector2D averageVelocity = Vector2D(0.f);

	for (auto& currAgent : agents)
	{
		if (Vector2D::Distance(currAgent->position, agent->position) < NEIGHBOR_RADIUS)
		{
			averageVelocity += currAgent->velocity;
			neighborCount++;
		}
	}

	averageVelocity /= neighborCount;
	alignmentDirection = Vector2D::Normalize(averageVelocity);

	//Combo
	flockingForce = (separationDirection * K_SEPARATION_FORCE)
				  + (cohesionDirection * K_COHESION_FORCE)
			      + (alignmentDirection * K_ALIGNMENT_FORCE);

	flockingForce *= K_MAX_FLOCKING_FORCE;
	
	return flockingForce;
}


Vector2D SteeringBehavior::Fishing(Agent *agent, std::vector<Agent*> agents,std::vector<Agent*> rocks, Vector2D target, float dtime)
{
	Vector2D separationDirection, alignmentDirection, targetDirection, fishingForce;

	for (int i = 0; i < (int)agents.size(); i++) {

		if (Vector2D::Distance(agent->position, target) < TARGET_RADIUS)
		{
			targetDirection += Vector2D::Normalize(agent->position - target);
		}

		for (int j = 0; j < (int)rocks.size(); j++) {
			if (Vector2D::Distance(rocks[j]->position, agent->position) < TARGET_RADIUS)
			{
				targetDirection += Vector2D::Normalize(rocks[j]->position - target);
			}
		}
	}

	///Separation;
	Vector2D separationVector = Vector2D(0.f);
	int neighborCount = 0;
	for (int i = 0; i < (int)agents.size(); i++) {

		if (Vector2D::Distance(agent->position, agents[i]->position) < FISH_RADIUS)
		{
			separationVector += agent->position - agents[i]->position;
			neighborCount++;
		}
	}

	separationVector /= neighborCount;
	separationDirection = Vector2D::Normalize(separationVector);

	//Combo
	fishingForce = (targetDirection * TARGET_FORCE_FISH)
		+ (separationDirection * SEPARATION_FORCE_FISH);

	fishingForce *= K_MAX_FISHING_FORCE;


	return fishingForce;
}
