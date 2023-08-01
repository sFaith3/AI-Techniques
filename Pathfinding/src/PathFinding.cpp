#include "PathFinding.h"

PathFinding::PathFinding()
{
	InitDebug();
	resetDebugNumbers();
}

PathFinding::~PathFinding()
{
}

Path PathFinding::GetPath(Node* start, Node* goal, std::unordered_map<Node*, Node*> cameFrom)
{
	Node* current = goal;
	Vector2D currentPos = Vector2D((float)current->position.first, (float)current->position.second);

	Path path;
	path.points.push_back(currentPos);

	while (current != start) {
		current = cameFrom[current];
		currentPos = Vector2D((float)current->position.first, (float)current->position.second);
		path.points.push_back(currentPos);
	}
	path.points.push_back(Vector2D((float)start->position.first, (float)start->position.second));
	std::reverse(path.points.begin(), path.points.end());
	path.points.push_back(Vector2D((float)goal->position.first, (float)goal->position.second));

	return path;
}

void PathFinding::InitDebug()
{
	iterations = 0;
	
	time = clock();
}

Path PathFinding::BFS(Graph * graph, Node * start, Node * goal, bool earlyExit)
{
	// Initialize the border with the node at the initial position.
	std::queue<Node*> frontier;
	frontier.push(start);

	std::unordered_map<Node*, Node*> cameFrom;
	std::unordered_map<Node*, Node*>::iterator itCameFrom;
	cameFrom[start] = nullptr;

	Node* current;

	while (!frontier.empty()) {
		// Take and remove an N node from the border.
		current = frontier.front();
		frontier.pop();

		if (earlyExit && current == goal) {
			break;
		}

		// Extends the border by adding the neightbors of N that have not yet visited, indicating the nodes from which they come.
		for (auto& next : current->nodesConnection) {
			itCameFrom = cameFrom.find(next);
			if (itCameFrom == cameFrom.end()) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
		iterations++;
	}

	return GetPath(start, goal, cameFrom);
}

Path PathFinding::Dijkstra(Graph * graph, Node * start, Node * goal)
{
	// Initialize the border with the node at the initial position.
	std::priority_queue<std::tuple<Node*, int>, std::vector<std::tuple<Node*, int>>, costComparison> frontier;

	std::tuple<Node*, int> currPriorityNode(start, 0);
	frontier.push(currPriorityNode);

	std::unordered_map<Node*, Node*> cameFrom;
	cameFrom[start] = nullptr;

	std::unordered_map<Node*, int> costSoFar;
	std::unordered_map<Node*, int>::iterator itCostSoFar;
	costSoFar[start] = 0;

	int newCost = 0;
	int priority = 0;

	Node* current;

	while (!frontier.empty()) {
		currPriorityNode = frontier.top();
		current = std::get<0>(currPriorityNode);
		frontier.pop();

		// Early exit.
		if (current == goal)
			break;

		for (auto& next : current->nodesConnection) {
			newCost = costSoFar[current] + graph->GetCost(next);
			itCostSoFar = costSoFar.find(next);
			if (itCostSoFar == costSoFar.end() || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				priority = newCost;
				currPriorityNode = std::make_tuple(next, priority);
				frontier.push(currPriorityNode);
				cameFrom[next] = current;
			}

		}
		iterations++;
	}

	return GetPath(start, goal, cameFrom);
}

Path PathFinding::GBFS(Graph * graph, Node * start, Node * goal)
{
	// Initialize the border with the node at the initial position.
	std::priority_queue<std::tuple<Node*, int>, std::vector<std::tuple<Node*, int>>, costComparison> frontier;
	std::tuple<Node*, int> currPriorityNode(start, 0);
	frontier.push(currPriorityNode);

	std::unordered_map<Node*, Node*> cameFrom;
	std::unordered_map<Node*, Node*>::iterator itCameFrom;
	cameFrom[start] = nullptr;

	int priority = 0;

	Node* current;

	while (!frontier.empty()) {
		currPriorityNode = frontier.top();
		current = std::get<0>(currPriorityNode);
		frontier.pop();

		// Early exit.
		if (current == goal)
			break;

		for (auto& next : current->nodesConnection) {
			itCameFrom = cameFrom.find(next);
			if (itCameFrom == cameFrom.end()) {
				priority = manhattan(goal, next); 
				currPriorityNode = std::make_tuple(next, priority);
				frontier.push(currPriorityNode);
				cameFrom[next] = current;
			}
		}
		iterations++;
	}

	return GetPath(start, goal, cameFrom);
}

Path PathFinding::A(Graph * graph, Node * start, Node * goal)
{
	// Initialize the border with the node at the initial position.
	std::priority_queue<std::tuple<Node*, int>, std::vector<std::tuple<Node*, int>>, costComparison> frontier;
	std::tuple<Node*, int> currPriorityNode(start, 0);
	frontier.push(currPriorityNode);

	std::unordered_map<Node*, Node*> cameFrom;
	cameFrom[start] = nullptr;

	std::unordered_map<Node*, int> costSoFar;
	std::unordered_map<Node*, int>::iterator itCostSoFar;
	costSoFar[start] = 0;

	int newCost = 0;
	int priority = 0;
	
	Node* current;

	while (!frontier.empty()) {
		currPriorityNode = frontier.top();
		current = std::get<0>(currPriorityNode);
		frontier.pop();

		// Early exit.
		if (current == goal)
			break;

		for (auto& next : current->nodesConnection) {
			newCost = costSoFar[current] + graph->GetCost(next);
			itCostSoFar = costSoFar.find(next);
			if (itCostSoFar == costSoFar.end() || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				priority = newCost + manhattan(goal, next);
				currPriorityNode = std::make_tuple(next, priority);
				frontier.push(currPriorityNode);
				cameFrom[next] = current;
			}
		}			
		iterations++;
	}

	return GetPath(start, goal, cameFrom);
}

Path PathFinding::Multitarget(Graph* graph, Graph* graphTerrain, const Node* _start)
{
	combinations.clear();
	calculatedPaths.clear();

	// Start gets actor node in simplified graph (numConections = numCoins).
	Node* start = graph->GetNode(_start->position);

	std::cout << "Building Tree..." << std::endl;
	treeNode* treeStart = new treeNode();
	treeStart->self = start;
	ExploreTree(treeStart);

	std::cout << "Tree built: " << "\nTime: " << ((float)clock() - time) / CLOCKS_PER_SEC << "s" << std::endl;
	std::cout << "Searching best path... " << std::endl;

	Path fullPath;
	bool firstIteration = true;
	for (auto& next : combinations) {

		treeNode* tmp = next;
		treeNode* parentTmp = next->parent;
		Path tmpPath;
		std::vector<Vector2D> tmpPathPoints;

		while (parentTmp != nullptr) {
			if (calculatedPaths.count(std::pair<Node*, Node*>(tmp->self, parentTmp->self)) == 0) {
				calculatedPaths[std::pair<Node*, Node*>(tmp->self, parentTmp->self)] =
					A(graphTerrain, graphTerrain->GetNode(tmp->self->position), graphTerrain->GetNode(parentTmp->self->position));
			}

			tmpPathPoints = calculatedPaths[std::pair<Node*, Node*>(tmp->self, parentTmp->self)].points;
			tmpPath.points.insert(tmpPath.points.end(), tmpPathPoints.begin(), tmpPathPoints.end());

			if (tmpPath.points.size() > fullPath.points.size() && !firstIteration) {
				break;
			}

			tmp = tmp->parent;
			parentTmp = parentTmp->parent;
		}

		if (tmpPath.points.size() < fullPath.points.size() || firstIteration) {
			fullPath = tmpPath;
			firstIteration = false;
		}
	}

	std::reverse(fullPath.points.begin(), fullPath.points.end());

	return fullPath;
}

void PathFinding::ExploreTree(treeNode * t)
{
	std::vector<Node*> tmpVisited;
	treeNode* tmp = t;

	while (tmp != nullptr){
		tmpVisited.push_back(tmp->self);
		tmp = tmp->parent;
	}

	std::vector<Node*> tmpNodesConnection = t->self->nodesConnection;
	std::vector<Node*>::iterator it;

	for (auto& v : tmpVisited) {
		it = std::find(tmpNodesConnection.begin(), tmpNodesConnection.end(), v);
		if (it != tmpNodesConnection.end())
		tmpNodesConnection.erase(it);
	}
	
	if (!tmpNodesConnection.empty()) {		
		for (auto& next : tmpNodesConnection) {
			treeNode* tmp2 = new treeNode();
			tmp2->self = next;
			tmp2->parent = t;
			ExploreTree(tmp2);

		}
	}
	else {
		combinations.push_back(t);
	}
}

float PathFinding::manhattan(Node* goal, Node* n)
{
	return abs(n->position.first - goal->position.first) + abs(n->position.second - goal->position.second);
}

void PathFinding::PrintDebug()
{
	time = clock() - time;

	if (minNodesExplored == 0) {
		minNodesExplored = iterations;
	}
	else if (minNodesExplored > iterations) {
		minNodesExplored = iterations;
	}

	if (maxNodesExplored < iterations){
		maxNodesExplored = iterations;
	}

	averageNodesExplored = (int)((maxNodesExplored + minNodesExplored) / 2.0f);

	std::cout << "Nodes explored: " << iterations << "\nTime: " << ((float)time) / CLOCKS_PER_SEC << "s" << std::endl;
	std::cout << "Min Nodes " << minNodesExplored << " - Max Nodes " << maxNodesExplored << " - Average Nodes Explored " << averageNodesExplored << std::endl;
}

void PathFinding::resetDebugNumbers()
{
	minNodesExplored = maxNodesExplored = averageNodesExplored = 0;
}
