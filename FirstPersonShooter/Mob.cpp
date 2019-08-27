#include "Mob.h"

Mob::Mob(EngineFPS *engine) : DynamicObject(engine) {}

Mob::~Mob() {}

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