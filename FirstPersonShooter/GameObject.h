#pragma once

class EngineFPS;
class Sprite;

class GameObject
{
public:
	GameObject(EngineFPS *engine);
	virtual ~GameObject();

	float x, y;
	Sprite *texture = nullptr;

protected:
	EngineFPS *engine = nullptr;
};