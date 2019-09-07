#pragma once

#include "Enemy.h"

class SS : public Enemy
{
public:
	SS(EngineFPS* engine, float x, float y);

	void OnHit(int damage) override;
};