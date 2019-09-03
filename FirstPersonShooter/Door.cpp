#include "Door.h"
#include "EngineFPS.h"

Door::Door(EngineFPS *engine, Sprite* texture) : Wall(engine, texture) {}

void Door::OnInteract()
{
	engine->PlayAudio("Door");
	removed = true;
}