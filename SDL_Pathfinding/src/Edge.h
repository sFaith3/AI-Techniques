#pragma once
#include "Node.h"

class Edge
{
public:

	Node* fromNode; 
	float cost;

	Edge();
	~Edge();

	// Returns the non-negative cost of the connection
	float GetCost();

	// Returns the node that this connection came from
	Node* GetFromNode();

	// Returns the node that this connection leads to
	Node* GetToNode();
};

