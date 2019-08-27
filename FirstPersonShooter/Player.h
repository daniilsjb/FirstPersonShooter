#pragma once

#include "Mob.h"

struct Player : Mob
{
	Player(EngineFPS *engine);

	void OnUpdate(float elapsedTime) override;
};