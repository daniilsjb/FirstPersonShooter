#include "ExitDoor.h"
#include "EngineFPS.h"

ExitDoor::ExitDoor(EngineFPS* engine, float x, float y, Sprite* spr) : Wall(engine, x, y, spr) {}

void ExitDoor::OnInteract()
{
	engine->PlayAudio("Switch");
	engine->FinishLevel();
}