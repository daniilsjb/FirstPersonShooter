#pragma once

#include "Mob.h"

class Weapon;

class Player : public Mob
{
public:
	Player(EngineFPS *engine);
	~Player();

	bool AddWeapon(Weapon *weapon);
	bool AddAmmoFromWeapon(Weapon *weapon);

	void OnUpdate(float elapsedTime) override;
	void OnHit(int damage) override;

	void AddScore(int amount);
	int GetScore() const;

	std::vector<Weapon*> availableWeapons;

private:
	int score = 0;
};