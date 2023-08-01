#include "SceneGOAP.h"
#include "FSMState.h"
#include "EnterMineAndDig.h"

using namespace std;

SceneGOAP::SceneGOAP()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze("res/maze.csv");
	loadTextures("res/maze.png", "res/coin.png");

	startWorldVariables();
	
	Action tmpAction = { "Start" };
	Node* startNode = new Node{ tmpAction,nullptr,0 };

	startNode->worldState = worldVariables;

	initNodes();

	cout << endl;
	cout << "*************" << endl;
	cout << endl;
	std::vector<Node*> path = GoapAstar(startNode, generateGoal());

	 
	// Print path.
	for (int i = 0; i < path.size(); i++) {

		std::cout << i+1 << "- " << path[i]->action.name << std::endl;
	}

	if (path.size() > 0) {
		if (path.back()->worldState["Enemy_Alive"] == condition::FALSE) { cout << "Enemy is dead" << endl; };
	}

	if (startNode->worldState["Enemy_Alive"] == condition::FALSE) { cout << "Enemy is dead" << endl; };
	if (startNode->worldState["Agent_Alive"] == condition::FALSE) { cout << "Agent has committed suicide" << endl; };


	cout << endl;
	cout << "PRESS 2 TO RESTART" << endl;
}

SceneGOAP::~SceneGOAP()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneGOAP::update(float dtime, SDL_Event *event)
{
}

void SceneGOAP::draw()
{
	drawMaze();
}

const char* SceneGOAP::getTitle()
{
	return "Decisions :: GOAP";
}

void SceneGOAP::initNodes()
{
	Action tmpAction;
	Node* tmpNode;

	tmpAction = { "Approach" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE)),
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Bomb", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemy_Nearby", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Scout" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE)),
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Reload" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Weapon", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Weapon_Reloaded", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Weapon_Reloaded", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Aim" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Weapon_Reloaded", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemy_Aligned", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Shoot" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Weapon", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Weapon_Reloaded", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Aligned", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemy_Alive", condition::FALSE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Detonate" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Nearby", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Bomb", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemy_Alive", condition::FALSE));
	tmpNode = new Node{ tmpAction,nullptr,30 };
	nodes.push_back(tmpNode);

	tmpAction = { "Flee" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Weapon", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Agent_Alive", condition::FALSE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Search Weapon" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Weapon", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Agent_with_Weapon", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Search Bomb" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemy_Visible", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Alive", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_with_Bomb", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Agent_with_Bomb", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,20 };
	nodes.push_back(tmpNode);
}

void SceneGOAP::startWorldVariables()
{
	worldVariables["Agent_Alive"] = condition::TRUE;
	worldVariables["Agent_with_Weapon"] = generateRandomCondition();
	worldVariables["Weapon_Reloaded"] = generateRandomCondition();
	worldVariables["Agent_with_Bomb"] = generateRandomCondition();
	worldVariables["Enemy_Visible"] = generateRandomCondition();
	worldVariables["Enemy_Aligned"] = generateRandomCondition();
	worldVariables["Enemy_Nearby"] = generateRandomCondition();
	worldVariables["Enemy_Alive"] = condition::TRUE;


	cout << "Initial World State" << endl;
	cout << "-------------------" << endl;
	cout << "Agent_Alive: " << getTrueFalseString(worldVariables["Agent_Alive"]) << endl;
	cout << "Agent_with_Weapon: " << getTrueFalseString(worldVariables["Agent_with_Weapon"]) << endl;
	cout << "Weapon_Reloaded: " << getTrueFalseString(worldVariables["Weapon_Reloaded"]) << endl;
	cout << "Agent_with_Bomb: " << getTrueFalseString(worldVariables["Agent_with_Bomb"]) << endl;
	cout << "Enemy_Visible: " << getTrueFalseString(worldVariables["Enemy_Visible"]) << endl;
	cout << "Enemy_Nearby: " << getTrueFalseString(worldVariables["Enemy_Nearby"]) << endl;
	cout << "Enemy_Aligned: " << getTrueFalseString(worldVariables["Enemy_Aligned"]) << endl;
	cout << "Enemy_Alive: " << getTrueFalseString(worldVariables["Enemy_Alive"]) << endl;
}



Node* SceneGOAP::generateGoal()
{
	std::unordered_map<std::string, condition> tmpWorldVariables;

	tmpWorldVariables["Agent_Alive"] = condition::DONTCARE;
	tmpWorldVariables["Agent_with_Weapon"] = condition::DONTCARE;
	tmpWorldVariables["Weapon_Reloaded"] = condition::DONTCARE;
	tmpWorldVariables["Agent_with_Bomb"] = condition::DONTCARE;
	tmpWorldVariables["Enemy_Visible"] = condition::DONTCARE;
	tmpWorldVariables["Enemy_Aligned"] = condition::DONTCARE;
	tmpWorldVariables["Enemy_Nearby"] = condition::DONTCARE;
	tmpWorldVariables["Enemy_Alive"] = condition::FALSE;

	Node* goal = new Node();

	goal->worldState = tmpWorldVariables;

	return goal;
}

condition SceneGOAP::generateRandomCondition()
{
	condition tmp;
	switch (rand() % 2) {

	case 0: tmp = condition::TRUE;
		break;
	case 1: tmp = condition::FALSE;
		break;
	}

	return tmp;
}

std::string SceneGOAP::getTrueFalseString(condition c)
{
	string tmp;
	if (c == condition::TRUE) tmp = "TRUE";
	if (c == condition::FALSE) tmp = "FALSE";
	if (c == condition::DONTCARE) tmp = "DON'T CARE";

	return tmp;
}

std::vector<Node*> SceneGOAP::getPossibleActions(Node* n)
{
	std::vector<Node*> connections;

	// It goes through all the nodes.
	for (int i = 0; i < nodes.size(); i++) {
		// It compares whether the precondition of the node that we are querying matches the world state of the node we have passed
		// as an attribute.
		int counter = nodes[i]->action.preConditions.size();
		if (n != nodes[i]) {
			for (int j = 0; j < nodes[i]->action.preConditions.size(); j++) {
				if (nodes[i]->action.preConditions[j].second == n->worldState[nodes[i]->action.preConditions[j].first]) {
					counter--;
				}
			}

			if (counter == 0) {
				connections.push_back(nodes[i]);
			}
		}
	}

	// Return all nodes that meet its preconditions.
	return connections;
}

void SceneGOAP::setNewWorldState(Node* n)
{
	n->worldState = n->parent->worldState;

	for (int i = 0; i < n->action.effects.size(); i++) {
		n->worldState[n->action.effects[i].first] = n->action.effects[i].second;
	}
}

int SceneGOAP::getHeuristic(Node* curr, Node* goal)
{
	int h = 0;
	for (std::pair<std::string, condition> it : goal->worldState) {
		if (it.second != condition::DONTCARE) {
			if (it.second != curr->worldState[it.first]) {
				h++;
			}
		}
	}

	return h;
}

std::vector<Node*> SceneGOAP::getPath(Node* goal)
{
	std::vector<Node*> path;
	Node* tmp = goal;

	while (tmp->parent != nullptr)
	{
		path.push_back(tmp);
		tmp = tmp->parent;
	}

	std::reverse(path.begin(), path.end());

	return path;
}

std::vector<Node*> SceneGOAP::GoapAstar(Node* start, Node* goal)
{
	std::priority_queue<std::tuple<Node*, int>, std::vector<std::tuple<Node*, int>>, costComparison> frontier;
	std::tuple<Node*, int> currPriorityNode(start, 0);
	frontier.push(currPriorityNode);

	std::unordered_map<Node*, Node*> cameFrom;
	cameFrom[start] = nullptr;

	std::unordered_map<Node*, int> costSoFar;
	std::unordered_map<Node*, int>::iterator itCostSoFar;
	costSoFar[start] = 0;

	int newCost = 0;
	int priority = 0;

	Node* current;

	while (!frontier.empty()) {
		currPriorityNode = frontier.top();
		current = std::get<0>(currPriorityNode);
		frontier.pop();

		// Early exit.	
		if (getHeuristic(current, goal) == 0) {
			return getPath(current);
		}

		std::vector<Node*> neighbours = getPossibleActions(current);
		for (auto& next : neighbours) {
			newCost = costSoFar[current] + next->g;
			itCostSoFar = costSoFar.find(next);

			if (itCostSoFar == costSoFar.end() || newCost < costSoFar[next]) {
				next->parent = current;
				setNewWorldState(next);
				costSoFar[next] = newCost;
				priority = newCost + getHeuristic(next, goal);

				currPriorityNode = std::make_tuple(next, priority);
				frontier.push(currPriorityNode);
				cameFrom[next] = current;
			}
		}
	}

	cout << "There is no possible path" << endl;
	return getPath(goal);
}

void SceneGOAP::drawMaze()
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
	SDL_Rect rect;
	Vector2D coords;

	for (int j = 0; j < num_cell_y; j++)
	{
		for (int i = 0; i < num_cell_x; i++)
		{
			switch (terrain[j][i])
			{
			case 0:
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
				break;
			case 1: // Do not draw if it is not necessary (bg is already black).
			default:
				continue;
			}

			coords = cell2pix(Vector2D(i, j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
			rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			// Alternative: render a tile texture
			//SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), tile_textures[0], .... );
		}
	}
	// Alternative: render a backgroud texture
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void SceneGOAP::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void SceneGOAP::initMaze(char* filename)
{
	// Initialize the terrain matrix from file (for each cell a zero value indicates it's a wall, positive values indicate terrain
	// cell cost).
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line))
	{
		vector<int> terrain_row;
		std::stringstream lineStream(line); 
		std::string cell;
		while (std::getline(lineStream, cell, ','))
			terrain_row.push_back(atoi(cell.c_str()));
		SDL_assert(terrain_row.size() == num_cell_x);
		terrain.push_back(terrain_row);
	}
	SDL_assert(terrain.size() == num_cell_y);
	infile.close();
}

bool SceneGOAP::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D SceneGOAP::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D SceneGOAP::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x/CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool SceneGOAP::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= terrain.size()) || (cell.x >= terrain[0].size()) )
		return false;
	return !(terrain[(unsigned int)cell.y][(unsigned int)cell.x] == 0);
}