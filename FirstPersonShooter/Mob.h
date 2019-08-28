#pragma once

#include "DynamicObject.h"
#include "Weapon.h"

struct Mob : DynamicObject
{
	Mob(EngineFPS *engine);
	virtual ~Mob();

	Weapon *weapon = nullptr;

	int GetMaxHealth();
	int GetHealth();

	void Heal(int amount);
	void Damage(int amount);

protected:
	int maxHealth;
	int currentHealth;
};