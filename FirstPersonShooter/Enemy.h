#pragma once

#include "Mob.h"

struct Enemy : Mob
{
	Enemy(EngineFPS *engine);

protected:
	const short DIRECTIONS_COUNT = 4;
	
	enum DIRECTION { BACK = 0, RIGHT, FRONT, LEFT };

	std::vector<Sprite*> directionSprites;

	Sprite* ChooseDirectionSprite(DynamicObject* relativeObject);
};

struct Guard : Enemy
{
	Guard(EngineFPS *engine);

	void OnUpdate(float elapsedTime) override;
	void OnHit(int damage) override;
};