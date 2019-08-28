#include "Mob.h"
#include "Weapon.h"
#include <iostream>

Mob::Mob(EngineFPS *engine) : DynamicObject(engine) {}

Mob::~Mob()
{
	delete weapon;
	weapon = nullptr;
}

int Mob::GetMaxHealth()
{
	return maxHealth;
}

int Mob::GetHealth()
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