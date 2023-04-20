#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "Agent.h"

class SceneDecisions :
	public Scene
{
public:
	SceneDecisions();
	~SceneDecisions();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	int num_cell_x;
	int num_cell_y;
	bool draw_grid;

	void initMaze(char* filename);
	std::vector< std::vector<int> > terrain;

	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);
	
	void drawMaze();
	void drawCoin();
	void drawCoins();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

};
