#pragma once
#include <vector> 
#include <cmath>
#include <unordered_map>
#include <iostream>
#include "Vector2D.h" 

struct coorHash //struct for the hash table of the unordered_map with key pair
{
	std::size_t operator()(const std::pair<int, int> &c) const{
		return ((std::hash<int>()(c.first) ^ (std::hash<int>()(c.second) << 1)) >> 1);
	}
};

struct Node {
	std::pair<int, int> position;
	int originalCost, cost;
	std::vector<Node*> nodesConnection;

	Node(int valueX = 0, int valueY = 0, int valueCost = 1) : position{ valueX, valueY },
															  originalCost{ valueCost },
															  cost{ valueCost }
	{}

	void print() {
		std::cout << "x:"<< position.first << "  -  y:" << position.second << std::endl;
	}
};

class Graph
{
private:
	std::unordered_map< std::pair<int, int>, Node*, coorHash > nodesMap;
	std::vector< std::vector<int> > terrain;
	int size;

	// Returns an array of connections outgoing from the given node
	std::vector<Node*> GetConnections(std::pair<int, int> _position, bool diagonals = false);
	
public:
	Graph(std::vector< std::vector<int> > _terrain);
	Graph(std::vector<Vector2D> positions);
	~Graph();

	Node* GetNode(std::pair<int, int> _position);
	int GetCost(Node* node);
	int GetSize();

	void ResetCosts();
	void SetCosts(std::unordered_map< std::pair<int, int>, int, coorHash > newCosts);
};