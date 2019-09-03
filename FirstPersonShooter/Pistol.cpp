#include "Pistol.h"
#include "EngineFPS.h"
#include "Mob.h"

Pistol::Pistol(EngineFPS *engine, Mob *parent) : Weapon(engine, parent)
{
	sprIdle = engine->GetSprite("Pistol");
	sprFire = engine->GetSprite("Pistol Fire");

	currentSpr = sprIdle;

	ammo = capacity = 20;

	minDmg = 3;
	maxDmg = 5;

	cooldown = 0.25f;
}

void Pistol::Fire()
{
	if (shooting || ammo <= 0) return;

	shooting = true;
	ammo--;

	float rayX, rayY, distance;
	if (engine->CastRay(parent->x, parent->y, parent->GetAngle(), rayX, rayY, distance, true, true, parent))
	{
		DynamicObject *other = engine->GetDynamicObject(rayX, rayY);
		if (other != nullptr && other->IsFriendly() != parent->IsFriendly())
			other->OnHit(rand() % maxDmg + minDmg);

		engine->PlayAudio("Pistol");
	}
}