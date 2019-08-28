#include "Weapon.h"
#include "EngineFPS.h"
#include "Mob.h"

Weapon::Weapon(EngineFPS *engine, Mob *parent, short index) : engine(engine), parent(parent), WEAPON_INDEX(index) {}

Weapon::~Weapon() {}

void Weapon::OnUpdate(float elapsedTime)
{
	if (shooting)
	{
		currentSpr = sprFire;
		timer += elapsedTime;
		if (timer >= cooldown)
		{
			timer = 0.0f;
			shooting = false;
		}
	}
	else
		currentSpr = sprIdle;
}

int Weapon::GetAmmo()
{
	return ammo;
}

int Weapon::GetCapacity()
{
	return capacity;
}

void Weapon::AddAmmo(int amount)
{
	ammo = min(capacity, ammo + amount);
}

Gun::Gun(EngineFPS *engine, Mob *parent) : Weapon(engine, parent, 0)
{
	sprIdle = engine->sprites["gun"];
	sprFire = engine->sprites["gun fire"];

	currentSpr = sprIdle;

	ammo = capacity = 20;

	minDmg = 3;
	maxDmg = 5;

	cooldown = 0.25f;
}

void Gun::Fire()
{
	if (shooting || ammo <= 0) return;

	shooting = true;
	ammo--;

	float rayX, rayY, distance;
	if (engine->CastRay(parent->x, parent->y, parent->angle, rayX, rayY, distance, false, true, parent))
	{
		DynamicObject *other = engine->GetDynamicObject(rayX, rayY);
		other->OnHit(rand() % maxDmg + minDmg);
	}
}