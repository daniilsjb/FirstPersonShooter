#include "SecretEntrance.h"
#include "EngineFPS.h"

SecretEntrance::SecretEntrance(EngineFPS* engine, float x, float y, Sprite* spr) : Wall(engine, x, y, spr) {}

void SecretEntrance::OnInteract()
{
	engine->PlayAudio("Secret Entrance");
	removed = true;
}