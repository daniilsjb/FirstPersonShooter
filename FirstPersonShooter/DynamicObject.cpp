#include "DynamicObject.h"
#include "EngineFPS.h"

DynamicObject::DynamicObject(EngineFPS* engine) : GameObject(engine) {}

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