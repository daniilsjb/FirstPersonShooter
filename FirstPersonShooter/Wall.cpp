#include "Wall.h"
#include "EngineFPS.h"

Wall::Wall(EngineFPS* engine, float x, float y, Sprite* spr) : GameObject(engine, x, y)
{
	texture = spr;
}

Wall::~Wall() {}

bool Wall::IsRemoved() const
{
	return removed;
}

void Wall::OnInteract() {}