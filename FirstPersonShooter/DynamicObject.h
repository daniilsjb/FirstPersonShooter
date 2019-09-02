#pragma once

#include "GameObject.h"

struct DynamicObject : GameObject
{
	DynamicObject(EngineFPS *engine);
	virtual ~DynamicObject();

	bool removed = false;
	bool friendlyToPlayer = false;

	float speed = 1.0f;
	float angle = 0.0f;

	virtual void OnUpdate(float elapsedTime) = 0;
	virtual void OnHit(int damage) = 0;
};