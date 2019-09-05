#include "MachineGun.h"
#include "EngineFPS.h"
#include "Mob.h"

MachineGun::MachineGun(EngineFPS* engine, Mob* parent) : Weapon(engine, parent)
{
	readySpr = engine->GetSprite("Machine Gun");
	shootingSpr = engine->GetSprite("Machine Gun Fire");
	cooldownSpr = engine->GetSprite("Machine Gun Cooldown");

	currentSpr = readySpr;

	ammo = 25;

	minDmg = 1;
	maxDmg = 2;

	shooting = 0.055f;
	cooldown = 0.015f;
}

void MachineGun::OnFireHeld()
{
	Fire();
}

void MachineGun::Fire()
{
	if (state != READY || ammo <= 0) return;

	ammo--;
	state = SHOOTING;

	float rayX, rayY, distance;
	if (engine->CastRay(parent->x, parent->y, parent->GetAngle(), rayX, rayY, distance, true, true, parent))
	{
		DynamicObject* other = engine->GetDynamicObject(rayX, rayY);
		if (other != nullptr && other->IsFriendly() != parent->IsFriendly())
			other->OnHit(rand() % maxDmg + minDmg);
	}

	engine->PlayAudio("Machine Gun");
}