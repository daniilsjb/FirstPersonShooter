#pragma once

#include "GameObject.h"

class DynamicObject : public GameObject
{
public:
	DynamicObject(EngineFPS* engine, float x, float y);
	virtual ~DynamicObject();

	virtual void OnUpdate(float elapsedTime) = 0;
	virtual void OnHit(int damage) = 0;

	bool IsRemoved() const;
	bool IsFriendly() const;

	float GetSpeed() const;
	float GetAngle() const;

protected:
	bool removed = false;
	bool friendlyToPlayer = false;

	float speed = 1.0f;
	float angle = 0.0f;
};