#include "Wall.h"

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

Door::Door(EngineFPS *engine, Sprite* texture) : Wall(engine, texture) {}

void Door::OnInteract()
{
	engine->PlayAudio("Door");
	removed = true;
}