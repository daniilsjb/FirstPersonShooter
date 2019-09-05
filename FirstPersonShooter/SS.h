#pragma once

#include "Enemy.h"

class SS : public Enemy
{
public:
	SS(EngineFPS* engine);

	void OnHit(int damage) override;
};