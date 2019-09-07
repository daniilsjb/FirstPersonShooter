#pragma once

#include "Enemy.h"

class Guard : public Enemy
{
public:
	Guard(EngineFPS* engine, float x, float y);

	void OnHit(int damage) override;
};