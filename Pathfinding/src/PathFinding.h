#pragma once
#include <queue>
#include <tuple>
#include <ctime>
#include <functional>
#include "Graph.h"
#include "Path.h"

class Agent;

enum class ENUM_PATH_TYPE {
	BFS, Dijkstra, GBFS, A
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

struct treeNode {
	treeNode* parent;
	Node* self;
	std::vector<treeNode*> connections;
};

struct nodeHash //struct for the hash table of the unordered_map with key pair
{
	std::size_t operator()(const std::pair<Node*, Node*> &c) const {
		return ((std::hash<Node*>()(c.first) ^ (std::hash<Node*>()(c.second) << 1)) >> 1);
	}
};

class PathFinding
{
private:
	int iterations;
	clock_t time;
	float manhattan(Node* goal, Node* current);
	std::vector<treeNode*> combinations;
	std::unordered_map<std::pair<Node*,Node*>, Path, nodeHash> calculatedPaths;
	int minNodesExplored, maxNodesExplored;
	int averageNodesExplored;

public:
	PathFinding();
	~PathFinding();
	
	void InitDebug();
	void PrintDebug();
	void resetDebugNumbers();
	Path GetPath(Node* start, Node* goal, std::unordered_map<Node*, Node*> cameFrom);

	Path BFS(Graph* graph, Node* start, Node* goal, bool earlyExit = false);
	Path Dijkstra(Graph* graph, Node* start, Node* goal);
	Path GBFS(Graph* graph, Node* start, Node* goal);
	Path A(Graph* graph, Node* start, Node* goal);

	Path Multitarget(Graph* graph, Graph * graphTerrain, const Node* start);
	void ExploreTree(treeNode* t);
};