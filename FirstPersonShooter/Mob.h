#pragma once

#include "DynamicObject.h"

struct Mob : DynamicObject
{
	Mob(EngineFPS *engine);
	virtual ~Mob();

	float maxHealth;
	float currentHealth;
};