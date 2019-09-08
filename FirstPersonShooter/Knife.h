#pragma once

#include "Weapon.h"

class Knife : public Weapon
{
public:
	Knife(EngineFPS* engine, Mob* parent);

	void OnFirePressed() override;

	void Fire() override;
};