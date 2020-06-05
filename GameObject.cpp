#include "GameObject.h"
#include "EngineFPS.h"

GameObject::GameObject(EngineFPS* engine, float x, float y) : engine(engine), x(x), y(y) {}

GameObject::~GameObject() {}

float GameObject::GetX() const
{
	return x;
}

float GameObject::GetY() const
{
	return y;
}