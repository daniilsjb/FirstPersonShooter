#pragma once

#include "Mob.h"

class Enemy : public Mob
{
public:
	Enemy(EngineFPS *engine);

protected:
	enum { PATROL = 0, COMBAT, FOLLOW } state;

	bool shootingDelayed = true;
	float shootingDelay = 0.0f;
	float delayTimer = 0.25f;

	const short DIRECTIONS_COUNT = 4;
	
	enum DIRECTION { BACK = 0, RIGHT, FRONT, LEFT };

	std::vector<Sprite*> directionSprites;
	Sprite *reloadingSpr;
	Sprite *shootingSpr;

	Sprite* ChooseDirectionSprite(DynamicObject* relativeObject);
};

struct Guard : Enemy
{
	Guard(EngineFPS *engine);

	void OnUpdate(float elapsedTime) override;
	void OnHit(int damage) override;

protected:
	bool playerDetected = false;
};