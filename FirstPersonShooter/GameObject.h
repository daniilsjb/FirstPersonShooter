#pragma once

#include "EngineFPS.h"

class GameObject
{
public:
	virtual ~GameObject();

	EngineFPS *engine = nullptr;

	float x, y;
	Sprite *texture = nullptr;

	GameObject(EngineFPS *engine);
};