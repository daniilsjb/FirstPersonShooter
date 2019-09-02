#pragma once

#include "Mob.h"

class Enemy : public Mob
{
public:
	Enemy(EngineFPS *engine);

	void OnUpdate(float elapsedTime) override;

protected:
	enum { PATROL = 0, COMBAT, FOLLOW } state;

	bool playerDetected = false;

	const short DIRECTIONS_COUNT = 4;
	enum DIRECTION { BACK = 0, RIGHT, FRONT, LEFT };

	bool shootingDelayed = true;
	float shootingDelay = 0.0f;
	float delayTimer = 0.25f;

	std::vector<Sprite*> directionSprites;
	Sprite *reloadingSpr;
	Sprite *shootingSpr;

	Sprite* ChooseDirectionSprite(DynamicObject* relativeObject);
};

struct Guard : public Enemy
{
public:
	Guard(EngineFPS *engine);

	void OnHit(int damage) override;
};