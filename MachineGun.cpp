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

	minDmg = 3;
	maxDmg = 8;

	shooting = 0.04f;
	cooldown = 0.01f;
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
	if (engine->CastRay(parent->GetX(), parent->GetY(), parent->GetAngle(), rayX, rayY, distance, true, true, parent))
	{
		DynamicObject* other = engine->GetDynamicObject(rayX, rayY);
		if (other != nullptr && other->IsFriendly() != parent->IsFriendly())
			other->OnHit(rand() % maxDmg + minDmg);
	}

	engine->PlayAudio("Machine Gun");
}