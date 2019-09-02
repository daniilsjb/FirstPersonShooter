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

bool Weapon::Ready() const
{
	return !shooting;
}

int Weapon::GetAmmo() const
{
	return ammo;
}

int Weapon::GetCapacity() const
{
	return capacity;
}

void Weapon::AddAmmo(int amount)
{
	ammo = min(capacity, ammo + amount);
}

Gun::Gun(EngineFPS *engine, Mob *parent) : Weapon(engine, parent, 0)
{
	sprIdle = engine->GetSprite("Pistol");
	sprFire = engine->GetSprite("Pistol Fire");

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
	if (engine->CastRay(parent->x, parent->y, parent->GetAngle(), rayX, rayY, distance, true, true, parent))
	{
		DynamicObject *other = engine->GetDynamicObject(rayX, rayY);
		if (other != nullptr && other->IsFriendly() != parent->IsFriendly())
			other->OnHit(rand() % maxDmg + minDmg);

		engine->PlayAudio("Pistol");
	}
}

MachineGun::MachineGun(EngineFPS *engine, Mob *parent) : Weapon(engine, parent, 1)
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