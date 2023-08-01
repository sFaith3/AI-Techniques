#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "utils.h"

class ScenePathFindingEnemies :
	public Scene
{
public:
	ScenePathFindingEnemies();
	~ScenePathFindingEnemies();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

private:
	Agent *agent;
	Vector2D coinPosition;

	std::vector<Agent*> enemies;
	int numEnemies;

	int num_cell_x;
	int num_cell_y;
	bool draw_grid;

	std::vector<std::vector<int>> terrain;
	Graph *graph;

	ENUM_PATH_TYPE currPathTypeEnum;

	const float distToCheckEnemy = 200.f;
	const float cdChangeMapCost = .3f;
	float currCdChangeMapCost;
	bool canCheckChangeMapCost;

	// Once the new path has been calculated to be based on the distance between the enemies, a short time is left to
	// recalculate again, in order to avoid, if possible, a higher path. Considering delay, 'cdChangeMapCost' + 'timeToRecalculate'.
	const float timeToRecalculate = .5f;
	float currTimeToRecalculate;
	bool isRecalculated;

	void initMaze(char* filename);
	std::vector<std::vector<int>> loadTerrain(char* filename);

	void InitEnemies();

	void drawMaze();
	void drawCoin();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

	bool isValidCell(Vector2D cell);

	// In order not to perform a new pathfinding once it has been recalculated when encountering a nearby enemy without having spent
	// seconds have passed.
	void UpdateCostMap(float dtime);
	bool CheckChangeMapCost(float dtime);
	bool CheckEnemiesNearby();
	void ModifyMapCost();
	void ChangeMapCost();
	void AreaExpansion(std::unordered_map<std::pair<int, int>, int, coorHash> &newCostsForEnemies, const Vector2D pos, const int cost);
};
