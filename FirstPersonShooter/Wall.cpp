#include "Wall.h"
#include "EngineFPS.h"

Wall::Wall(EngineFPS* engine, Sprite* spr) : GameObject(engine)
{
	texture = spr;
}

Wall::~Wall() {}

bool Wall::IsRemoved() const
{
	return removed;
}

void Wall::OnInteract() {}