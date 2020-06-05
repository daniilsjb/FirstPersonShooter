#include "Knife.h"
#include "EngineFPS.h"
#include "Mob.h"

Knife::Knife(EngineFPS* engine, Mob* parent) : Weapon(engine, parent)
{
	readySpr = engine->GetSprite("Knife");
	shootingSpr = engine->GetSprite("Knife Fire");
	cooldownSpr = engine->GetSprite("Knife Cooldown");

	currentSpr = readySpr;

	ammo = Weapons::CAPACITY;

	minDmg = 1;
	maxDmg = 4;

	shooting = 0.25f;
	cooldown = 0.05f;
}

void Knife::OnFirePressed()
{
	Fire();
}

void Knife::Fire()
{
	if (state != READY) return;

	state = SHOOTING;

	float rayX, rayY, distance;
	if (engine->CastRay(parent->GetX(), parent->GetY(), parent->GetAngle(), rayX, rayY, distance, true, true, parent))
	{
		if (distance <= 2.0f)
		{
			DynamicObject* other = engine->GetDynamicObject(rayX, rayY);
			if (other != nullptr && other->IsFriendly() != parent->IsFriendly())
				other->OnHit(rand() % maxDmg + minDmg);
		}
	}

	engine->PlayAudio("Knife");
}