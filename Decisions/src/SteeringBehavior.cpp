#include "SteeringBehavior.h"

using namespace std;

SteeringBehavior::SteeringBehavior()
{
}

SteeringBehavior::~SteeringBehavior()
{
}

Vector2D SteeringBehavior::Seek(Agent *agent, Vector2D target, float dtime)
{
	Vector2D desired_velocity = target - agent->position;
	desired_velocity.Normalize();
	desired_velocity *= agent->max_velocity;

	Vector2D steering_force = desired_velocity - agent->velocity;
	steering_force /= agent->max_velocity;
	steering_force = steering_force * agent->max_force;

	return steering_force;
}

Vector2D SteeringBehavior::Seek(Agent *agent, Agent *target, float dtime)
{
	return Seek(agent, target->position, dtime);
}

Vector2D SteeringBehavior::Arrive(Agent *agent, Vector2D target, int slow_radius, float dtime)
{
	float slow_factor = (target - agent->position).Length() / slow_radius;

	Vector2D desired_velocity = target - agent->position;
	desired_velocity.Normalize();
	if ((target - agent->position).Length() > slow_radius)
		desired_velocity *= agent->max_velocity;
	else
		desired_velocity *= agent->max_velocity * slow_factor;

	Vector2D steering_force = desired_velocity - agent->velocity;
	//steering_force /= agent->max_velocity;
	//steering_force = steering_force * agent->max_force;

	return steering_force;
}

Vector2D SteeringBehavior::Arrive(Agent *agent, Agent *target, int slow_radius, float dtime)
{
	return Arrive(agent, target->position, slow_radius, dtime);
}

Vector2D SteeringBehavior::SimplePathFollowing(Agent *agent, float dtime)
{
	//There is a path and we have not started to follow it
	if ((agent->currentTargetIndex == -1) && (agent->path.points.size()>0))
		agent->setCurrentTargetIndex(0); 

	//We are in the middle of a path
	if (agent->getCurrentTargetIndex() >= 0)
	{
		float dist = Vector2D::Distance(agent->getPosition(), agent->path.points[agent->currentTargetIndex]);
		// We 've reached the current target
		if (dist < agent->path.ARRIVAL_DISTANCE)
		{
			// Current target is path's last point?
			if (agent->getCurrentTargetIndex() == agent->path.points.size() - 1)
			{
				if (dist < 3) // We've reached the end of the path
				{
					agent->path.points.clear();
					agent->setCurrentTargetIndex(-1);
					agent->setVelocity(Vector2D(0, 0));
					return Vector2D(0,0);
				}
				else
				{
					return Arrive(agent, agent->getTarget(), agent->path.ARRIVAL_DISTANCE, dtime);
				}
			}
			// Go to the next point in the path
			agent->setCurrentTargetIndex(agent->currentTargetIndex+1);
		}
		// Seek to current target
		agent->setTarget(agent->path.points[agent->currentTargetIndex]);
		return Seek(agent, agent->getTarget(), dtime);
	}
	// Do nothing if there is no path!
	return Vector2D(0, 0);  
}