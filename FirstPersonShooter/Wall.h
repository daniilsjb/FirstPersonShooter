#pragma once

#include "GameObject.h"

struct Wall : GameObject
{
	Wall(EngineFPS *engine, Sprite* texture);
	virtual ~Wall();

	bool removed = false;

	virtual void OnInteract();
};