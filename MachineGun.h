#pragma once

#include "Weapon.h"

class MachineGun : public Weapon
{
public:
	MachineGun(EngineFPS* engine, Mob* parent);

	void OnFireHeld() override;

	void Fire() override;
};
