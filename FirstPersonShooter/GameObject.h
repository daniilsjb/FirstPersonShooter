#pragma once

#include "EngineFPS.h"

struct GameObject
{
	virtual ~GameObject();

	EngineFPS *engine = nullptr;

	float x, y;
	Sprite *texture = nullptr;

	GameObject(EngineFPS *engine);
};