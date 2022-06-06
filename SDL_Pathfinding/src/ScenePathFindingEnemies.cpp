#include "ScenePathFindingEnemies.h"

using namespace std;

ScenePathFindingEnemies::ScenePathFindingEnemies()
{
	cout << "\n\n\n-------------------------------------------------------\n\n" << endl;

	srand((unsigned int)time(NULL));

	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	
	currPathTypeEnum = ENUM_PATH_TYPE::A;

	currCdChangeMapCost = cdChangeMapCost;
	canCheckChangeMapCost = true;

	isRecalculated = true;
	currTimeToRecalculate = timeToRecalculate;

	initMaze("../res/costsMaze.csv");
	loadTextures("../res/maze.png", "../res/coin.png");

	agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setTarget(Vector2D(-20, -20));

	InitEnemies();
	
	// set agent position coords to the center of a random cell
	Vector2D rand_cell = (-1, -1);
	while (!isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agent->setPosition(cell2pix(rand_cell));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1, -1);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

	agent->pathToPos(coinPosition, graph, currPathTypeEnum);
}

ScenePathFindingEnemies::~ScenePathFindingEnemies()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);
	if (agent)
		delete agent;

	for (int i = 0; i < (int)enemies.size(); i++)
	{
		if (enemies[i])
			delete enemies[i];
	}
}

void ScenePathFindingEnemies::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
	default:
		break;
	}

	Vector2D steering_force;
	Agent* enemy;

	for (int i = 0; i < numEnemies; i++) {
		enemy = enemies[i];
		steering_force = enemy->Behavior()->SimplePathFollowing(enemy, dtime, false);
		enemy->update(steering_force, dtime, event);
	}

	steering_force = agent->Behavior()->SimplePathFollowing(agent, dtime);
	agent->update(steering_force, dtime, event);

	// if we have arrived to the coin, replace it in a random cell!
	if ((agent->getCurrentTargetIndex() == -1) && (pix2cell(agent->getPosition()) == coinPosition))
	{
		coinPosition = Vector2D(-1, -1);
		while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agent->getPosition())) < 3))
			coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

		if (!CheckChangeMapCost(dtime))
			agent->pathToPos(coinPosition, graph, currPathTypeEnum);
	}
	else {
		UpdateCostMap(dtime);
	}
}

void ScenePathFindingEnemies::draw()
{
	drawMaze();
	drawCoin();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	agent->draw();

	for (int i = 0; i < numEnemies; i++) {
		enemies[i]->draw();
	}
}

const char* ScenePathFindingEnemies::getTitle()
{
	return "SDL Path Finding :: PathFinding Enemies";
}

void ScenePathFindingEnemies::drawMaze()
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
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 0, 255);

				break;
			default:
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), terrain[j][i] * 2, 0, 0, 255);
				break;
			}

			coords = cell2pix(Vector2D((float)i, (float)j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
			rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			//Alternative: render a tile texture:
			//SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), tile_textures[0], .... );
		}
	}
	//Alternative: render a backgroud texture:
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void ScenePathFindingEnemies::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void ScenePathFindingEnemies::initMaze(char* filename)
{
	terrain = loadTerrain(filename);
	graph = new Graph(terrain);
}

std::vector<std::vector<int>> ScenePathFindingEnemies::loadTerrain(char * filename)
{
	// Initialize the terrain matrix from file (for each cell a zero value indicates it's a wall, positive values indicate terrain cell cost)
	std::ifstream infile(filename);
	std::string line;
	std::vector<std::vector<int>> newTerrain;

	while (std::getline(infile, line))
	{
		vector<int> terrain_row;
		std::stringstream lineStream(line);
		std::string cell;
		while (std::getline(lineStream, cell, ','))
			terrain_row.push_back(atoi(cell.c_str()));
		SDL_assert(terrain_row.size() == num_cell_x);
		newTerrain.push_back(terrain_row);
	}
	SDL_assert(newTerrain.size() == num_cell_y);
	infile.close();

	return newTerrain;
}

void ScenePathFindingEnemies::InitEnemies()
{
	numEnemies = 3;
	Agent *enemy;
	Vector2D positions[] = {
		Vector2D(50, 50), Vector2D(100, 50), Vector2D(50, 100)
	};
	Vector2D patrols[] = {
		Vector2D(75, 100), Vector2D(25, 25), Vector2D(25, 40)
	};
	Vector2D pos;

	for (int i = 0; i < numEnemies; i++) {
		enemy = new Agent;

		pos = positions[i];
		while (!isValidCell(pos))
			pos = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
		enemy->setPosition(cell2pix(pos));
		enemy->pathToPos(pos, graph, currPathTypeEnum);

		pos = patrols[i];
		while (!isValidCell(pos))
			pos = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
		enemy->pathToPos(pos, graph, currPathTypeEnum, false);

		enemy->loadSpriteTexture("../res/zombie1.png", 8);
		enemy->setTarget(Vector2D(-20, -20));
		enemies.push_back(enemy);
	}
}

bool ScenePathFindingEnemies::loadTextures(char* filename_bg, char* filename_coin)
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

bool ScenePathFindingEnemies::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= terrain.size()) || (cell.x >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.y][(unsigned int)cell.x] == 0);
}

void ScenePathFindingEnemies::UpdateCostMap(float dtime)
{
	if (canCheckChangeMapCost) {
		if (CheckChangeMapCost(dtime)) {
			canCheckChangeMapCost = false;
		}
	}
	else {
		currCdChangeMapCost -= dtime;
		if (currCdChangeMapCost <= 0.f) {
			canCheckChangeMapCost = true;
			isRecalculated = false;
			currCdChangeMapCost = cdChangeMapCost;
		}
	}
}

bool ScenePathFindingEnemies::CheckChangeMapCost(float dtime)
{
	if (CheckEnemiesNearby()) {
		ModifyMapCost();

		return true;
	}
	else if (!isRecalculated) {
		currTimeToRecalculate -= dtime;
		if (currTimeToRecalculate <= 0.f) {
			ModifyMapCost();
			isRecalculated = true;
			currTimeToRecalculate = timeToRecalculate;
		}
	}

	return false;
}

bool ScenePathFindingEnemies::CheckEnemiesNearby()
{
	bool changeMapCost = false;
	for (int i = 0; i < numEnemies; i++) {
		if (agent->Behavior()->IsOtherAgentClose(agent, enemies[i], distToCheckEnemy)) {
			changeMapCost = true;
			break;
		}
	}

	return changeMapCost;
}

void ScenePathFindingEnemies::ModifyMapCost()
{
	ChangeMapCost();
	agent->pathToPos(coinPosition, graph, currPathTypeEnum);
	graph->ResetCosts();
}

void ScenePathFindingEnemies::ChangeMapCost()
{
	Vector2D pos;
	std::pair<int, int> keyPos;
	const int costEnemy = 120;
	std::unordered_map<std::pair<int, int>, int, coorHash> newCostsForEnemies;

	for (int i = 0; i < enemies.size(); i++) {
		pos = pix2cell((enemies[i]->getPosition()));
		keyPos = std::pair<int, int>((int)pos.y, (int)pos.x);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, costEnemy));
		}
		AreaExpansion(newCostsForEnemies, pos, costEnemy);
	}
	graph->SetCosts(newCostsForEnemies);
}

void ScenePathFindingEnemies::AreaExpansion(std::unordered_map<std::pair<int, int>, int, coorHash> &newCostsForEnemies, const Vector2D pos, const int cost)
{
	const int area = 5;
	Vector2D currPos = pos;
	std::pair<int, int> keyPos;
	int currCost = cost;

	for (int i = 1; i <= area; i++) {
		currCost -= (int)((float)currCost * (0.25f));
		
		keyPos = std::pair<int, int>((int)currPos.y + i, (int)currPos.x);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y - i, (int)currPos.x);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y, (int)currPos.x + i);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y, (int)currPos.x - i);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y + i, (int)currPos.x + i);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y - i, (int)currPos.x + i);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y + i, (int)currPos.x - i);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}

		keyPos = std::pair<int, int>((int)currPos.y - i, (int)currPos.x - i);
		if (graph->GetNode(keyPos)) {
			newCostsForEnemies.insert(std::pair<std::pair<int, int>, int>(keyPos, currCost));
		}
	}
}
