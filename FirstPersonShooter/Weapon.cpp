#include "Weapon.h"
#include "EngineFPS.h"
#include "Mob.h"

Weapon::Weapon(EngineFPS* engine, Mob* parent) : engine(engine), parent(parent) {}

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

bool Weapon::IsFull() const
{
	return (ammo >= capacity);
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

void Weapon::RestoreAmmo(float percent)
{
	AddAmmo((int)(percent * capacity));
}