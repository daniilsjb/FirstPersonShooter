#pragma once

class EngineFPS;
class Sprite;

class GameObject
{
public:
	GameObject(EngineFPS* engine, float x, float y);
	virtual ~GameObject();

	Sprite* texture = nullptr;

	float GetX() const;
	float GetY() const;

protected:
	EngineFPS* engine;

	float x, y;
};