#include "Wall.h"
#include "EngineFPS.h"

Wall::Wall(EngineFPS *engine, Sprite* texture) : GameObject(engine)
{
	this->texture = texture;
}

Wall::~Wall() {}

bool Wall::IsRemoved() const
{
	return removed;
}

void Wall::OnInteract() {}