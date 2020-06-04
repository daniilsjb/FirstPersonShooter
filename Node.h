#pragma once

#include <vector>

struct Node
{
	int x, y;
	bool obstacle = false;
	float heuristic = FLT_MAX;
	std::vector<Node*> neighbors;
};