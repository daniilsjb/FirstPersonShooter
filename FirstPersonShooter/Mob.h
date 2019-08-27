#pragma once

#include "DynamicObject.h"

struct Mob : DynamicObject
{
	Mob(EngineFPS *engine);
	virtual ~Mob();

	int GetMaxHealth();
	int GetHealth();

	void Heal(int amount);
	void Damage(int amount);

protected:
	int maxHealth;
	int currentHealth;
};