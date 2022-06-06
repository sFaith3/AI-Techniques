#include "Agent.h"

using namespace std;

Agent::Agent() : sprite_texture(0),
                 position(Vector2D(100, 100)),
	             target(Vector2D(1000, 100)),
	             velocity(Vector2D(0,0)),
	             mass(0.1f),
	             max_force(150),
	             max_velocity(200),
	             orientation(0),
	             color({ 255,255,255,255 }),
				 sprite_num_frames(0),
	             sprite_w(0),
	             sprite_h(0),
	             draw_sprite(false),
				 currentTargetIndex(-1)
{
	steering_behavior = new SteeringBehavior;
	path_finding = new PathFinding;
}

Agent::~Agent()
{
	if (sprite_texture)
		SDL_DestroyTexture(sprite_texture);
	if (steering_behavior)
		delete (steering_behavior);
	if (path_finding)
		delete (path_finding);
}

void Agent::addPathPoint(Vector2D point)
{
	if (path.points.size() > 0)
		if (path.points[path.points.size() - 1] == point)
			return;

	path.points.push_back(point);
}

int Agent::getCurrentTargetIndex()
{
	return currentTargetIndex;
}

void Agent::setCurrentTargetIndex(int idx)
{
	currentTargetIndex = idx;
}

SteeringBehavior * Agent::Behavior()
{
	return steering_behavior;
}
PathFinding* Agent::PathBehavior()
{
	return path_finding;
}

Vector2D Agent::getPosition()
{
	return position;
}

Vector2D Agent::getTarget()
{
	return target;
}

Vector2D Agent::getVelocity()
{
	return velocity;
}

float Agent::getMaxVelocity()
{
	return max_velocity;
}

void Agent::setPosition(Vector2D _position)
{
	position = _position;
}

void Agent::setTarget(Vector2D _target)
{
	target = _target;
}

void Agent::setVelocity(Vector2D _velocity)
{
	velocity = _velocity;
}

void Agent::setMass(float _mass)
{
	mass = _mass;
}

void Agent::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	color = { r, g, b, a };
}

void Agent::update(Vector2D steering_force, float dtime, SDL_Event *event)
{
	switch (event->type) {
		/* Keyboard & Mouse events */
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_sprite = !draw_sprite;
		break;
	default:
		break;
	}

	Vector2D acceleration = steering_force / mass;
	velocity = velocity + acceleration * dtime;
	velocity = velocity.Truncate(max_velocity);

	position = position + velocity * dtime;

	// Update orientation
	if (velocity.Length()>0)
		orientation = (float)(atan2(velocity.y, velocity.x) * RAD2DEG);

	// Trim position values to window size
	if (position.x < 0) position.x = TheApp::Instance()->getWinSize().x;
	if (position.y < 0) position.y = TheApp::Instance()->getWinSize().y;
	if (position.x > TheApp::Instance()->getWinSize().x) position.x = 0;
	if (position.y > TheApp::Instance()->getWinSize().y) position.y = 0;
}

void Agent::draw()
{
	for (int i = 0; i < (int)path.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);

	if (draw_sprite)
	{
		Uint32 sprite;
		
		if (velocity.Length() < 5.0)
			sprite = 1;
		else
			sprite = (int)(SDL_GetTicks() / (-0.1*velocity.Length() + 250)) % sprite_num_frames;//sprite = (int)(SDL_GetTicks() / (max_velocity)) % sprite_num_frames;
		
		SDL_Rect srcrect = { (int)sprite * sprite_w, 0, sprite_w, sprite_h };
		SDL_Rect dstrect = { (int)position.x - (sprite_w / 2), (int)position.y - (sprite_h / 2), sprite_w, sprite_h };
		SDL_Point center = { sprite_w / 2, sprite_h / 2 };
		SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), sprite_texture, &srcrect, &dstrect, orientation+90, &center, SDL_FLIP_NONE);
	}
	else 
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)position.x, (int)position.y, 15, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)position.x, (int)position.y, (int)(position.x+15*cos(orientation*DEG2RAD)), (int)(position.y+15*sin(orientation*DEG2RAD)));
	}
}

bool Agent::loadSpriteTexture(char* filename, int _num_frames)
{
	if (_num_frames < 1) return false;

	SDL_Surface *image = IMG_Load(filename);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	sprite_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	sprite_num_frames = _num_frames;
	sprite_w = image->w / sprite_num_frames;
	sprite_h = image->h;
	draw_sprite = true;

	if (image)
		SDL_FreeSurface(image);

	return true;
}

void Agent::pathToPos(Vector2D target, Graph *graph, ENUM_PATH_TYPE pathType, bool clearPath)
{
	Node* startNode, *finalNode;
	Vector2D currPos;

	currPos = getPosition();
	currPos = pix2cell(Vector2D((float)(currPos.x), (float)(currPos.y)));
	startNode = graph->GetNode(std::pair<int, int>(currPos.y, currPos.x));
	finalNode = graph->GetNode(std::pair<int, int>(target.y, target.x)); // cell = finalPos

	/// add a new path got it with a pathfinding algorithm to agent from a target position
	Path newPath = Behavior()->GetPath(this, graph, startNode, finalNode, pathType);
	std::vector<Vector2D> newPoints = newPath.points;
	Vector2D pixelPos;

	if (clearPath)
		this->clearPath();

	for (int i = 0; i < newPath.points.size(); i++) {
		pixelPos = cell2pix(Vector2D((float)(newPoints[i].y), (float)(newPoints[i].x)));
		addPathPoint(pixelPos);
	}
}

void Agent::pathToPositions(std::vector<Vector2D> _targets, Graph *graph, ENUM_PATH_TYPE pathType)
{
	Vector2D agentPosition = getPosition();
	agentPosition = pix2cell(Vector2D((float)(agentPosition.x), (float)(agentPosition.y)));
	Node* startNode = graph->GetNode(std::pair<int, int>(agentPosition.y, agentPosition.x));
	std::vector<Node*> targets;
	std::vector<Vector2D> currPoses;

	/// add a new path got it with a pathfinding algorithm to agent from a target position
	_targets.push_back(agentPosition);
	Path newPath = Behavior()->GetPath(this, graph, startNode, _targets);
	std::vector<Vector2D> newPoints = newPath.points;
	Vector2D pixelPos;

	clearPath();
	for (int i = 0; i < newPath.points.size(); i++) {
		pixelPos = cell2pix(Vector2D((float)(newPoints[i].y), (float)(newPoints[i].x)));
		addPathPoint(pixelPos);
	}
}

void Agent::clearPath()
{
	path.points.clear();
	path.points.resize(0);
	setCurrentTargetIndex(-1);
}
