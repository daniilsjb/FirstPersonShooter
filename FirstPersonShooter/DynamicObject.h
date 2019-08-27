#pragma once

#include "GameObject.h"

struct DynamicObject : GameObject
{
	DynamicObject(EngineFPS *engine);
	virtual ~DynamicObject();

	bool removed = false;
	bool friendlyToPlayer = false;

	float speed;
	float angle;

	virtual void OnUpdate(float elapsedTime) = 0;
};