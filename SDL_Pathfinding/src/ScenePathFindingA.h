#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <iostream>
#include <deque>
#include "Graph.h"
#include "Scene.h"
#include "Agent.h"

class ScenePathFindingA :
	public Scene
{
public:
	ScenePathFindingA();
	~ScenePathFindingA();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	std::vector<Vector2D> coinPositions;

	const int minCoins = 4;
	const int maxCoins = 7;
	int numCoins;

	int num_cell_x;
	int num_cell_y;
	bool draw_grid;

	std::vector<std::vector<int>> terrain;
	Graph* graphTerrain;
	Graph* graphCoins;

	ENUM_PATH_TYPE currPathTypeEnum;

	void initMaze(char* filename);
	std::vector<std::vector<int>> loadTerrain(char* filename);
	void InitCoins(Vector2D rand_cell);

	void drawMaze();
	void drawCoins();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

	bool isValidCell(Vector2D cell);
};
