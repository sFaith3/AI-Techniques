#include "SceneDecisions.h"
#include "FSMState.h"
#include "EnterMineAndDig.h"

using namespace std;

SceneDecisions::SceneDecisions()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze("../res/maze.csv");
	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

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


	agents[0]->ChangeState(new EnterMineAndDig());
}

SceneDecisions::~SceneDecisions()
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

void SceneDecisions::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
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
	}


	
}

void SceneDecisions::draw()
{
	drawMaze();
	drawCoins();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	agents[0]->draw();
}

const char* SceneDecisions::getTitle()
{
	return "SDL Path Finding :: PathFinding Mouse Demo";
}

void SceneDecisions::drawMaze()
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

void SceneDecisions::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void SceneDecisions::drawCoins()
{
	Vector2D coin_coords;
	if (agents[0]->currState->GetName() == "BANK") {
		 coin_coords = Vector2D(SRC_WIDTH / 1.8f, 5.f * SRC_HEIGHT / 6.f);
	}
	else {
		coin_coords = Vector2D(SRC_WIDTH / 1.8f, SRC_HEIGHT / 5.f);
	}

	int offset = CELL_SIZE / 2;
	int distance = 10;

	
	for (int i = 0; i < agents[0]->gold/10; i++) {
		SDL_Rect dstrect = { (int)coin_coords.x - offset , (int)coin_coords.y - offset - distance * i, CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
	}
	
	coin_coords = Vector2D(SRC_WIDTH / 20.f, 5.f * SRC_HEIGHT / 6.f);

	int stackSize = 20;
	for (int i = 0; i < agents[0]->money / 5; i++) {
		SDL_Rect dstrect = { (int)coin_coords.x - offset + distance * 2 * (int)(i/ stackSize) , (int)coin_coords.y - offset - distance * (i % stackSize), CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
		
		if (i > 300)cout << i << endl;
	}
}

void SceneDecisions::initMaze(char* filename)
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

bool SceneDecisions::loadTextures(char* filename_bg, char* filename_coin)
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

Vector2D SceneDecisions::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D SceneDecisions::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x/CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool SceneDecisions::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= terrain.size()) || (cell.x >= terrain[0].size()) )
		return false;
	return !(terrain[(unsigned int)cell.y][(unsigned int)cell.x] == 0);
}