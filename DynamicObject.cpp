#include "DynamicObject.h"
#include "EngineFPS.h"

DynamicObject::DynamicObject(EngineFPS* engine, float x, float y) : GameObject(engine, x, y) {}

DynamicObject::~DynamicObject() {}

bool DynamicObject::IsRemoved() const
{
	return removed;
}

bool DynamicObject::IsFriendly() const
{
	return friendlyToPlayer;
}

float DynamicObject::GetSpeed() const
{
	return speed;
}

float DynamicObject::GetAngle() const
{
	return angle;
}