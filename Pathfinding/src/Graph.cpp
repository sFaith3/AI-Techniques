#include "Graph.h"

Graph::Graph(std::vector< std::vector<int> > _terrain)
{
	terrain = _terrain;
	int cost = 0;

	for (int i = 0; i < terrain.size(); i++) {
		for (int j = 0; j < terrain[i].size(); j++) {
			cost = terrain[i][j];
			if (cost != 0) {
				Node* tmp = new Node;
				tmp->position = std::pair<int, int>(i, j);
				tmp->cost = cost;
				nodesMap.insert(std::pair<std::pair<int, int>, Node*>(std::pair<int, int>(i, j), tmp));
			}
		}
	}

	for (int i = 0; i < terrain.size(); i++) {
		for (int j = 0; j < terrain[i].size(); j++) {
			cost = terrain[i][j];
			if (cost != 0) {
				if (nodesMap.count(std::pair<int, int>(i, j)) > 0) {
					nodesMap[std::pair<int, int>(i, j)]->nodesConnection = GetConnections(std::pair<int, int>(i, j));
				}
			}
		}
	}

	size = terrain.size() * terrain[0].size();
}

Graph::Graph(std::vector<Vector2D> positions) {
	for (int i = 0; i < positions.size(); i++) {
		Node* tmp = new Node();
		tmp->position = std::pair<int, int>(positions[i].y, positions[i].x);
		nodesMap[std::pair<int, int>(positions[i].y, positions[i].x)] = tmp;
	}

	for (int i = 0; i < positions.size(); i++){
		for (int j = 0; j < positions.size(); j++){
			if (i != j){
				nodesMap[std::pair<int, int>(positions[i].y, positions[i].x)]->nodesConnection.push_back(nodesMap[std::pair<int, int>(positions[j].y, positions[j].x)]);
			}
		}
	}

	size = positions.size();
}

Graph::~Graph()
{
}

Node* Graph::GetNode(std::pair<int, int> _position)
{
	if (nodesMap.count(_position) > 0){
		return nodesMap[_position];
	}
	else{
		return nullptr;
	}
}

std::vector<Node*> Graph::GetConnections(std::pair<int, int> _position,bool diagonals)
{
	std::vector<Node*> nodes;
	Node auxNode;

	int x = _position.first;
	int y = _position.second;
	int sizeTerrainX = terrain.size();
	int sizeTerrainY = terrain[0].size();

	if ((y >= 0 && y < sizeTerrainY) && (x >= 0 && x < sizeTerrainX)) {
		//Arriba
		if (x - 1 >= 0 && terrain[x - 1][y] != 0) {
			nodes.push_back(nodesMap[std::pair<int, int>(x - 1, y)]);
		}
		//Derecha
		if (y + 1 < sizeTerrainY && terrain[x][y + 1] != 0) {
			nodes.push_back(nodesMap[std::pair<int, int>( x,y + 1)]);
		}
		//Abajo
		if (x + 1 < sizeTerrainX && terrain[x + 1][y] != 0) {
			nodes.push_back(nodesMap[std::pair<int, int>(x + 1, y)]);
		}
		//Izquieda
		if (y - 1 >= 0 && terrain[x][y - 1] != 0) {
			nodes.push_back(nodesMap[std::pair<int, int>(x, y - 1)]);
		}
		if (diagonals) {
			//Diagonal Sup-Derecha
			if ((y + 1 < sizeTerrainY && x - 1 >= 0) && terrain[x - 1][y + 1] != 0) {
				nodes.push_back(nodesMap[std::pair<int, int>(x - 1, y + 1)]);
			}
			//Diagonal Inf-Derecha
			if ((y + 1 < sizeTerrainY && x + 1 < sizeTerrainX) && terrain[x + 1][y + 1] != 0) {
				nodes.push_back(nodesMap[std::pair<int, int>(x + 1, y + 1)]);
			}
			//Diagonal Inf-Izquierda
			if ((y - 1 >= 0 && x + 1 < sizeTerrainX) && terrain[x + 1][y - 1] != 0) {
				nodes.push_back(nodesMap[std::pair<int, int>(x + 1, y - 1)]);
			}
			//Diagonal Sup-Izquierda
			if ((y - 1 >= 0 && x - 1 >= 0) && terrain[x - 1][y - 1] != 0) {
				nodes.push_back(nodesMap[std::pair<int, int>(x - 1, y - 1)]);
			}
		}
	}
	
	return nodes;
}

int Graph::GetCost(Node * node)
{
	return node->cost;
}

int Graph::GetSize()
{
	return size;
}

void Graph::ResetCosts()
{
	for (auto& currNode : nodesMap) {
		currNode.second->cost = currNode.second->originalCost;
	}
}

void Graph::SetCosts(std::unordered_map< std::pair<int, int>, int, coorHash > newCosts)
{
	Node *currNode;
	for (auto& currCost : newCosts) {
		currNode = GetNode(currCost.first);
		if (currNode) {
			currNode->cost += currCost.second;
		}
	}
}
