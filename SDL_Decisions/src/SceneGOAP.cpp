#include "SceneGOAP.h"
#include "FSMState.h"
#include "EnterMineAndDig.h"

using namespace std;

SceneGOAP::SceneGOAP()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze("../res/maze.csv");
	loadTextures("../res/maze.png", "../res/coin.png");





	startWorldVariables();


	
	
	Action tmpAction = { "start" };
	Node* startNode = new Node{ tmpAction,nullptr,0 };

	startNode->worldState = worldVariables;

	initNodes();

	cout << endl;
	cout << "*************" << endl;
	cout << endl;
	std::vector<Node*> path = GoapAstar(startNode, generateGoal());

	
	 
	//Imprime path
	for (int i = 0; i < path.size(); i++) {

		std::cout << path[i]->action.name << std::endl;
	}

	cout << endl;

	if (path.size() > 0) {
		if (path.back()->worldState["Enemic_Viu"] == condition::FALSE) { cout << "L'enemic ha mort" << endl; };
	}

	if (startNode->worldState["Enemic_Viu"] == condition::FALSE) { cout << "L'enemic ha mort" << endl; };
	if (startNode->worldState["Agent_Viu"] == condition::FALSE) { cout << "L'agent s'ha suicidat" << endl; };


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
	return "SDL Path Finding :: PathFinding Mouse Demo";
}

void SceneGOAP::initNodes()
{
	Action tmpAction;
	Node* tmpNode;


	

	tmpAction = { "Apropar" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE)),
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Bomba", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemic_Aprop", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Explorar" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE)),
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Recarregar" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Arma", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Arma_Carregada", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Arma_Carregada", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Apuntar" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Arma_Carregada", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemic_Alineat", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);


	tmpAction = { "Disparar" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Arma", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Arma_Carregada", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Alineat", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemic_Viu", condition::FALSE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Detonar" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Aprop", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Bomba", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE));
	tmpAction.addEffect(std::pair<std::string, condition>("Enemic_Viu", condition::FALSE));
	tmpNode = new Node{ tmpAction,nullptr,30 };
	nodes.push_back(tmpNode);


	tmpAction = { "Fugir" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Arma", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Agent_Viu", condition::FALSE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);


	tmpAction = { "Buscar Arma" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Arma", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Agent_Te_Arma", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,10 };
	nodes.push_back(tmpNode);

	tmpAction = { "Buscar Bomba" };
	tmpAction.addPreCondition(std::pair<std::string, condition>("Enemic_Visible", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Viu", condition::TRUE));
	tmpAction.addPreCondition(std::pair<std::string, condition>("Agent_Te_Bomba", condition::FALSE));
	tmpAction.addEffect(std::pair<std::string, condition>("Agent_Te_Bomba", condition::TRUE));
	tmpNode = new Node{ tmpAction,nullptr,20 };
	nodes.push_back(tmpNode);



	
}

void SceneGOAP::startWorldVariables()
{
	//DEFAULT VARIABLES
	//worldVariables["Agent_Viu"] = condition::TRUE;
	//worldVariables["Agent_Te_Arma"] = condition::FALSE;
	//worldVariables["Arma_Carregada"] = condition::FALSE;
	//worldVariables["Agent_Te_Bomba"] = condition::FALSE;
	//worldVariables["Enemic_Visible"] = condition::FALSE;
	//worldVariables["Enemic_Alineat"] = condition::FALSE;
	//worldVariables["Enemic_Aprop"] = condition::FALSE;
	//worldVariables["Enemic_Viu"] = condition::TRUE;



	worldVariables["Agent_Viu"] = condition::TRUE;
	worldVariables["Agent_Te_Arma"] = generateRandomCondition();
	worldVariables["Arma_Carregada"] = generateRandomCondition();
	worldVariables["Agent_Te_Bomba"] = generateRandomCondition();
	worldVariables["Enemic_Visible"] = generateRandomCondition();
	worldVariables["Enemic_Alineat"] = generateRandomCondition();
	worldVariables["Enemic_Aprop"] = generateRandomCondition();
	worldVariables["Enemic_Viu"] = condition::TRUE;


	cout << "Initial World State" << endl;
	cout << "Agent_Viu: " << getTrueFalseString(worldVariables["Agent_Viu"]) << endl;
	cout << "Agent_Te_Arma: " << getTrueFalseString(worldVariables["Agent_Te_Arma"]) << endl;
	cout << "Arma_Carregada: " << getTrueFalseString(worldVariables["Arma_Carregada"]) << endl;
	cout << "Agent_Te_Bomba: " << getTrueFalseString(worldVariables["Agent_Te_Bomba"]) << endl;
	cout << "Enemic_Visible: " << getTrueFalseString(worldVariables["Enemic_Visible"]) << endl;
	cout << "Enemic_Aprop: " << getTrueFalseString(worldVariables["Enemic_Aprop"]) << endl;
	cout << "Enemic_Alineat: " << getTrueFalseString(worldVariables["Enemic_Alineat"]) << endl;
	cout << "Enemic_Viu: " << getTrueFalseString(worldVariables["Enemic_Viu"]) << endl;



}



Node* SceneGOAP::generateGoal()
{
	std::unordered_map<std::string, condition> tmpWorldVariables;

	tmpWorldVariables["Agent_Viu"] = condition::DONTCARE;
	tmpWorldVariables["Agent_Te_Arma"] = condition::DONTCARE;
	tmpWorldVariables["Arma_Carregada"] = condition::DONTCARE;
	tmpWorldVariables["Agent_Te_Bomba"] = condition::DONTCARE;
	tmpWorldVariables["Enemic_Visible"] = condition::DONTCARE;
	tmpWorldVariables["Enemic_Alineat"] = condition::DONTCARE;
	tmpWorldVariables["Enemic_Aprop"] = condition::DONTCARE;
	tmpWorldVariables["Enemic_Viu"] = condition::FALSE;

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
	
	//Recorre todos los nodos
	for (int i = 0; i < nodes.size(); i++) {
		//Compara si la precondicion del nodo que estamos consultando coinicide con el estado del mundo del nodo que le hemos pasado como atributo

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

	//Devuelve todos los nodos que cumplen con sus precondiciones
	return connections;
}

void SceneGOAP::setNewWorldState(Node * n)
{


	n->worldState = n->parent->worldState;


	for (int i = 0; i < n->action.effects.size(); i++) {
		n->worldState[n->action.effects[i].first] = n->action.effects[i].second;
	}
}

int SceneGOAP::getHeuristic(Node * curr, Node * goal)
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

std::vector<Node*> SceneGOAP::getPath(Node * goal)
{
	std::vector<Node*> path;
	Node * tmp = goal;

	while (tmp->parent != nullptr)
	{

		path.push_back(tmp);
		tmp = tmp->parent;
	}

	std::reverse(path.begin(), path.end());

	return path;
}

std::vector<Node*> SceneGOAP::GoapAstar(Node *start, Node *goal)
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

		//early exit
	
		if (getHeuristic(current, goal) == 0) {
			return getPath(current);
		}	
		

		
		std::vector<Node*> neighbours = getPossibleActions(current);

		for (auto& next : neighbours){
			
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


	
	cout << "No hi ha path possible" << endl;
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
			case 1: // Do not draw if it is not necessary (bg is already black)
			default:
				continue;
			}

			coords = cell2pix(Vector2D(i,j)) - Vector2D( (float)CELL_SIZE/2, (float)CELL_SIZE/2 );
			rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			//Alternative: render a tile texture:
			//SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), tile_textures[0], .... );
		}
	}
	//Alternative: render a backgroud texture:
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
	// Initialize the terrain matrix from file (for each cell a zero value indicates it's a wall, positive values indicate terrain cell cost)
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