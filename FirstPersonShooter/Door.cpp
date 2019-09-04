#include "Door.h"
#include "EngineFPS.h"

Door::Door(EngineFPS *engine, Sprite* spr) : Wall(engine, spr) {}

void Door::OnInteract()
{
	engine->PlayAudio("Door");
	removed = true;
}