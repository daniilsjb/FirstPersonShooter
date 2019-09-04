#pragma once

#include "Weapon.h"

class Pistol : public Weapon
{
public:
	Pistol(EngineFPS* engine, Mob* parent);

	void Fire() override;
};