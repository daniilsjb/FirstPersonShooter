#include "Mob.h"
#include "EngineFPS.h"
#include "Weapon.h"

Mob::Mob(EngineFPS* engine, float x, float y) : DynamicObject(engine, x, y) {}

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
	return (weapon != nullptr) ? weapon->currentSpr : nullptr;
}

int Mob::GetWeaponAmmo() const
{
	return (weapon != nullptr) ? weapon->GetAmmo() : 0;
}

bool Mob::FullAmmo() const
{
	return (weapon != nullptr) ? (weapon->GetAmmo() >= Weapons::CAPACITY) : true;
}

void Mob::RestoreAmmo()
{
	if (weapon!= nullptr)
		weapon->RestoreAmmo();
}

int Mob::GetMaxHealth() const
{
	return maxHealth;
}

int Mob::GetHealth() const
{
	return currentHealth;
}

bool Mob::FullHealth() const
{
	return (currentHealth >= maxHealth);
}

void Mob::RestoreHealth()
{
	currentHealth = maxHealth;
}

void Mob::Heal(int amount)
{
	currentHealth = min(maxHealth, currentHealth + amount);
}

void Mob::Damage(int amount)
{
	currentHealth = max(0, currentHealth - amount);
}

bool Mob::IsDead() const
{
	return (currentHealth <= 0);
}