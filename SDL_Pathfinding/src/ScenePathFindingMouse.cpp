#include "ScenePathFindingMouse.h"

using namespace std;

ScenePathFindingMouse::ScenePathFindingMouse()
{
	cout << "\n\n\n-------------------------------------------------------\n\n" << endl;

	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze("../res/costsMaze.csv");
	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));
	
	printIntroMsg();
	currPathTypeEnum = ENUM_PATH_TYPE::BFS;

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setTarget(Vector2D(-20,-20));
	agents.push_back(agent);

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1,-1);
	while (!isValidCell(rand_cell)) 
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1,-1);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell)<3)) 
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

	agents[0]->pathToPos(coinPosition, graph, currPathTypeEnum);
}

ScenePathFindingMouse::~ScenePathFindingMouse()
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

void ScenePathFindingMouse::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.scancode) {
		case SDL_SCANCODE_SPACE:
			draw_grid = !draw_grid;
			break;

		case SDL_SCANCODE_A:
			currPathTypeEnum = ENUM_PATH_TYPE::BFS;
			printCurrPathType("BFS");
			resetDebug();
			printIntroMsg();
			break;

		case SDL_SCANCODE_S:
			currPathTypeEnum = ENUM_PATH_TYPE::Dijkstra;
			printCurrPathType("Dijkstra");
			resetDebug();
			printIntroMsg();
			break;

		case SDL_SCANCODE_D:
			currPathTypeEnum = ENUM_PATH_TYPE::GBFS;
			printCurrPathType("GBFS");
			resetDebug();
			printIntroMsg();
			break;

		case SDL_SCANCODE_Z:
			currPathTypeEnum = ENUM_PATH_TYPE::A;
			printCurrPathType("A*");
			resetDebug();
			printIntroMsg();
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	Vector2D steering_force = agents[0]->Behavior()->SimplePathFollowing(agents[0], dtime);
	agents[0]->update(steering_force, dtime, event);

	// if we have arrived to the coin, replace it ina random cell!
	if ((agents[0]->getCurrentTargetIndex() == -1) && (pix2cell(agents[0]->getPosition()) == coinPosition))
	{
		coinPosition = Vector2D(-1, -1);
		while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agents[0]->getPosition()))<3))
			coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

		agents[0]->pathToPos(coinPosition, graph, currPathTypeEnum);
	}
	
}

void ScenePathFindingMouse::draw()
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
		for (int j = 0; j < SRC_HEIGHT; j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	agents[0]->draw();
}

const char* ScenePathFindingMouse::getTitle()
{
	return "SDL Path Finding :: PathFinding Mouse";
}

void ScenePathFindingMouse::drawMaze()
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
	SDL_Rect rect;
	Vector2D coords;

	for (int j = 0; j < num_cell_y; j++){
		for (int i = 0; i < num_cell_x; i++){
			switch (terrain[j][i]){
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

void ScenePathFindingMouse::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void ScenePathFindingMouse::initMaze(char* filename)
{
	terrain = loadTerrain(filename);
	graph = new Graph(terrain);
}

std::vector<std::vector<int>> ScenePathFindingMouse::loadTerrain(char* filename)
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

void ScenePathFindingMouse::resetDebug()
{
	agents[0]->PathBehavior()->resetDebugNumbers();
}

void ScenePathFindingMouse::printIntroMsg()
{
	std::cout << "\nPress 'A' to BFS path" << std::endl;
	std::cout << "Press 'S' to Dijkstra path" << std::endl;
	std::cout << "Press 'D' to GBFS path" << std::endl;
	std::cout << "Press 'Z' to A* path\n" << std::endl;
}

void ScenePathFindingMouse::printCurrPathType(const char * name)
{
	std::cout << "\nCurrent path type: " << name << std::endl;
}

bool ScenePathFindingMouse::loadTextures(char* filename_bg, char* filename_coin)
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

bool ScenePathFindingMouse::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= terrain.size()) || (cell.x >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.y][(unsigned int)cell.x] == 0);
}