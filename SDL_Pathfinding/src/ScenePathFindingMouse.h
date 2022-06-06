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

class ScenePathFindingMouse :
	public Scene
{
public:
	ScenePathFindingMouse();
	~ScenePathFindingMouse();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	int num_cell_x;
	int num_cell_y;
	bool draw_grid;
	
	std::vector<std::vector<int>> terrain;
	Graph* graph;

	ENUM_PATH_TYPE currPathTypeEnum;

	void initMaze(char* filename);
	std::vector<std::vector<int>> loadTerrain(char* filename);
	void resetDebug();
	void printIntroMsg();
	void printCurrPathType(const char* name);

	void drawMaze();
	void drawCoin();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

	bool isValidCell(Vector2D cell);
};
