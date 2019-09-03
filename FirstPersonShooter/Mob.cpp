#include "Mob.h"
#include "EngineFPS.h"
#include "Weapon.h"

Mob::Mob(EngineFPS *engine) : DynamicObject(engine) {}

Mob::~Mob()
{
	delete weapon;
	weapon = nullptr;
}

bool Mob::HasWeapon() const
{
	return (weapon != nullptr);
}

Sprite* Mob::GetWeaponSprite() const
{
	return weapon->currentSpr;
}

int Mob::GetWeaponAmmo() const
{
	return weapon->GetAmmo();
}

int Mob::GetWeaponCapacity() const
{
	return weapon->GetCapacity();
}

int Mob::GetMaxHealth() const
{
	return maxHealth;
}

int Mob::GetHealth() const
{
	return currentHealth;
}

void Mob::Heal(int amount)
{
	currentHealth = min(maxHealth, currentHealth + amount);
}

void Mob::Damage(int amount)
{
	currentHealth = max(0, currentHealth - amount);
}