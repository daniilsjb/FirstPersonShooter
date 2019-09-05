#pragma once

#include "Mob.h"

#include <vector>

class Enemy : public Mob
{
public:
	Enemy(EngineFPS* engine);

	void OnUpdate(float elapsedTime) override;

protected:
	enum { PATROL = 0, COMBAT, FOLLOW, PAIN } state = PATROL;

	bool playerDetected = false;

	enum DIRECTION { BACK = 0, RIGHT, FRONT, LEFT, DIRECTION_COUNT };

	float painTimer = 0.0f;
	float pain = 0.14f;

	bool shootingDelayed = true;
	float shootingDelay = 0.0f;
	float delayTimer = 0.25f;

	std::vector<Sprite*> directionSprites;
	Sprite* aimingSpr;
	Sprite* shootingSpr;
	Sprite* painSpr;

	Sprite* ChooseDirectionSprite(DynamicObject* relativeObject) const;
};