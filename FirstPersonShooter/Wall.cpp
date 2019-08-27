#include "Wall.h"

Wall::Wall(EngineFPS *engine, Sprite* texture) : GameObject(engine)
{
	this->texture = texture;
}

Wall::~Wall() {}

void Wall::OnInteract() {}