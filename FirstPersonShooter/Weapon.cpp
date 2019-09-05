#include "Weapon.h"
#include "EngineFPS.h"
#include "Mob.h"

Weapon::Weapon(EngineFPS* engine, Mob* parent) : engine(engine), parent(parent) {}

Weapon::~Weapon() {}

void Weapon::OnUpdate(float elapsedTime)
{
	switch (state)
	{
		case READY:
		{
			currentSpr = readySpr;
			break;
		}
		case SHOOTING:
		{
			currentSpr = shootingSpr;

			stateTimer += elapsedTime;
			if (stateTimer >= shooting)
			{
				stateTimer = 0.0f;
				state = COOLDOWN;
			}
			break;
		}
		case COOLDOWN:
		{
			currentSpr = cooldownSpr;

			stateTimer += elapsedTime;
			if (stateTimer >= cooldown)
			{
				stateTimer = 0.0f;
				state = READY;
			}
			break;
		}
	}
}

void Weapon::OnFirePressed() {}

void Weapon::OnFireHeld() {}

void Weapon::OnFireReleased() {}

bool Weapon::Ready() const
{
	return state == READY;
}

bool Weapon::IsFull() const
{
	return (ammo >= CAPACITY);
}

int Weapon::GetAmmo() const
{
	return ammo;
}

int Weapon::GetCapacity() const
{
	return CAPACITY;
}

void Weapon::AddAmmo(int amount)
{
	ammo = min(CAPACITY, ammo + amount);
}