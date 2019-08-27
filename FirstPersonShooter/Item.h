#pragma once

#include "GameObject.h"

struct Item : GameObject
{
	Item(EngineFPS *engine);
	virtual ~Item();

	bool removed = false;

	virtual void OnUse() = 0;
};