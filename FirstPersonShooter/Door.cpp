#include "Door.h"
#include "EngineFPS.h"

Door::Door(EngineFPS* engine, float x, float y, Sprite* spr) : Wall(engine, x, y, spr) {}

void Door::OnInteract()
{
	engine->PlayAudio("Door");
	removed = true;
}