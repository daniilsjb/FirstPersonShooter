#include "MachineGun.h"
#include "EngineFPS.h"
#include "Mob.h"

MachineGun::MachineGun(EngineFPS *engine, Mob *parent) : Weapon(engine, parent)
{
	sprIdle = engine->GetSprite("Machine Gun");
	sprFire = engine->GetSprite("Machine Gun Fire");

	currentSpr = sprIdle;

	ammo = capacity = 200;

	minDmg = 1;
	maxDmg = 2;

	cooldown = 0.01f;
}

void MachineGun::Fire()
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

		engine->PlayAudio("Machine Gun");
	}
}