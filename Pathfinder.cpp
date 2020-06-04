#include "Pathfinder.h"
#include "GameObject.h"
#include "Node.h"

#include <list>
#include <map>

Pathfinder::Pathfinder() {}

Pathfinder::~Pathfinder() {}

bool Pathfinder::FindMove(Node* graph, int graphWidth, int graphHeight, GameObject *start, GameObject *finish, float &x, float &y)
{
	//If start and finish are in same place, then there is no further move to make
	if ((int)start->GetX() == (int)finish->GetX() && (int)start->GetY() == (int)finish->GetY())
	{
		x = start->GetX();
		y = start->GetY();
		return true;
	}

	std::list<Node*> frontier;
	std::map<Node*, Node*> cameFrom;

	Node *nodeStart = &graph[graphWidth * (int)start->GetY() + (int)start->GetX()];
	Node *nodeFinish = &graph[graphWidth * (int)finish->GetY() + (int)finish->GetX()];

	//Set initial settings
	frontier.push_back(nodeStart);
	cameFrom[nodeStart] = nullptr;

	//Pathfinding algorithm itself
	while (!frontier.empty())
	{
		Node *nodeCurrent = frontier.front();
		frontier.pop_front();

		if (nodeCurrent == nodeFinish)
			break;

		for (auto &neighbor : nodeCurrent->neighbors)
		{
			if (!neighbor->obstacle && cameFrom.find(neighbor) == cameFrom.end())
			{
				neighbor->heuristic = (float)(fabs(nodeFinish->x - neighbor->x) + fabs(nodeFinish->y - neighbor->y));
				frontier.push_back(neighbor);
				cameFrom[neighbor] = nodeCurrent;
			}
		}

		frontier.sort([](const Node* a, const Node* b) { return a->heuristic < b->heuristic; });
	}

	//Construct the path if it exists (from finish to start)
	std::vector<Node*> path;

	if (cameFrom[nodeFinish] != nullptr)
	{
		Node *nodeCurrent = nodeFinish;
		while (nodeCurrent != nodeStart)
		{
			path.push_back(nodeCurrent);
			nodeCurrent = cameFrom[nodeCurrent];
		}
	}
	else
		return false;

	//Set the first move based on the first move in the path
	x = (float)path.back()->x;
	y = (float)path.back()->y;

	return true;
}