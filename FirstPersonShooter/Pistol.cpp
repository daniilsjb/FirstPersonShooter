#include "Pistol.h"
#include "EngineFPS.h"
#include "Mob.h"

Pistol::Pistol(EngineFPS* engine, Mob* parent) : Weapon(engine, parent)
{
	readySpr = engine->GetSprite("Pistol");
	shootingSpr = engine->GetSprite("Pistol Fire");
	cooldownSpr = engine->GetSprite("Pistol Cooldown");

	currentSpr = readySpr;

	ammo = 8;

	minDmg = 3;
	maxDmg = 5;

	shooting = 0.25f;
	cooldown = 0.05f;
}

void Pistol::OnFirePressed()
{
	Fire();
}

void Pistol::Fire()
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

	engine->PlayAudio("Pistol");
}