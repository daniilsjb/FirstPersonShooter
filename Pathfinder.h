#pragma once

#include <vector>

class GameObject;
struct Node;

class Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();

	bool FindMove(Node* graph, int graphWidth, int graphHeight, GameObject *start, GameObject *finish, float &x, float &y);
};