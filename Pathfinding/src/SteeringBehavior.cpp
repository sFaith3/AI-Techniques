#include "SteeringBehavior.h"

using namespace std;

SteeringBehavior::SteeringBehavior()
{
}

SteeringBehavior::~SteeringBehavior()
{
}

Path SteeringBehavior::GetPath(Agent * agent, Graph *graph, Node *start, Node *goal, ENUM_PATH_TYPE pathType)
{
	Path path;
	PathFinding* pathBehavior = agent->PathBehavior();

	switch (pathType) {
	case ENUM_PATH_TYPE::BFS:
		pathBehavior->InitDebug();
		path = pathBehavior->BFS(graph, start, goal, true);
		pathBehavior->PrintDebug();
		break;

	case ENUM_PATH_TYPE::Dijkstra:
		pathBehavior->InitDebug();
		path = pathBehavior->Dijkstra(graph, start, goal);
		pathBehavior->PrintDebug();
		break;

	case ENUM_PATH_TYPE::GBFS:
		pathBehavior->InitDebug();
		path = pathBehavior->GBFS(graph, start, goal);
		pathBehavior->PrintDebug();
		break;

	case ENUM_PATH_TYPE::A:
		pathBehavior->InitDebug();
		path = pathBehavior->A(graph, start, goal);
		pathBehavior->PrintDebug();
		break;

	default:
		break;
	}

	return path;
}

Path SteeringBehavior::GetPath(Agent * agent, Graph * graph, const Node * start , std::vector<Vector2D> goals)
{
	Path path;
	PathFinding* pathBehavior = agent->PathBehavior();
	Graph* goalGraph = new Graph(goals);
	
	pathBehavior->InitDebug();
	path = pathBehavior->Multitarget(goalGraph, graph, start);
	pathBehavior->PrintDebug();

	return path;
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

Vector2D SteeringBehavior::SimplePathFollowing(Agent *agent, float dtime, bool clearPath)
{
	//There is a path and we have not started to follow it
	if ((agent->currentTargetIndex == -1) && (agent->path.points.size() > 0))
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
					if (clearPath) {
						agent->path.points.clear();
					}
					else {
						reverse(agent->path.points.begin(), agent->path.points.end());
					}
					agent->setCurrentTargetIndex(-1);
					agent->setVelocity(Vector2D(0, 0));

					return Vector2D(0, 0);
				}
				else
				{
					return Arrive(agent, agent->getTarget(), agent->path.ARRIVAL_DISTANCE, dtime);
				}
			}
			// Go to the next point in the path
			agent->setCurrentTargetIndex(agent->currentTargetIndex + 1);
		}
		// Seek to current target
		agent->setTarget(agent->path.points[agent->currentTargetIndex]);
		return Seek(agent, agent->getTarget(), dtime);
	}
	// Do nothing if there is no path!
	return Vector2D(0, 0);
}

bool SteeringBehavior::IsOtherAgentClose(Agent * agent, Agent * other, float dist)
{
	return (Vector2D::Distance(agent->getPosition(), other->getPosition()) <= dist);
}
