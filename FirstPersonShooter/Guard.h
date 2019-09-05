#pragma once

#include "Enemy.h"

class Guard : public Enemy
{
public:
	Guard(EngineFPS* engine);

	void OnHit(int damage) override;
};