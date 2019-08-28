#pragma once

#include "Mob.h"

struct Weapon;

struct Player : Mob
{
	Player(EngineFPS *engine);
	~Player();

	bool AddWeapon(Weapon *weapon);

	void OnUpdate(float elapsedTime) override;
	void OnHit(int damage) override;

private:
	std::vector<Weapon*> availableWeapons;
};