#pragma once

#include "Enemy.h"

struct Guard : public Enemy
{
public:
	Guard(EngineFPS *engine);

	void OnHit(int damage) override;
};