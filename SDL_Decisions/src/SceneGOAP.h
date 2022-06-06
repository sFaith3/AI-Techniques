#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <unordered_map>
#include <queue>

#include "Scene.h"
#include "Agent.h"


enum class condition // 0 = false = FALSE, 1 = true = TRUE, 2 = DONTCARE 
{
	FALSE, TRUE, DONTCARE
};

struct Action {

	std::string name;

	//std::pair<std::string, condition> pre;
	std::vector< std::pair<std::string, condition>> preConditions;

	//std::pair<std::string, condition> post;
	std::vector< std::pair<std::string, condition>> effects;

	

	void addPreCondition(std::pair<std::string, condition> pre) { preConditions.push_back(pre); }
	void addEffect(std::pair<std::string, condition> post) { effects.push_back(post); }
};



struct Node {

	Action action;
	
	
	Node* parent;

	int g;
	int h;
	int f;

	std::unordered_map<std::string, condition> worldState;
};

class costComparison
{
public:
	costComparison()
	{
	}

	bool operator() (const std::tuple<Node*, int>& lhs, const std::tuple<Node*, int>&rhs) const
	{
		return (std::get<1>(lhs) > std::get<1>(rhs));
	}
};



class SceneGOAP :
	public Scene
{
public:
	SceneGOAP();
	~SceneGOAP();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;
	std::unordered_map<std::string, condition> worldVariables;

	std::vector<Node*> nodes;

	void initNodes();

	void startWorldVariables();

	Node* generateGoal();

	condition generateRandomCondition();

	std::string getTrueFalseString(condition c);

	std::vector<Node*> getPossibleActions(Node* n);

	void setNewWorldState(Node*n);

	int getHeuristic(Node* curr, Node* goal);
	
	std::vector<Node*> getPath(Node* goal);


	std::vector<Node*> GoapAstar(Node *start , Node* goal);


	//WorldVariables *worldVariables;

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
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

};
