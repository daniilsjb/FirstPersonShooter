#pragma once

#include "Mob.h"

#include <vector>

class Player : public Mob
{
public:
	Player(EngineFPS* engine, float x, float y);
	~Player();

	bool WeaponAcquired(short weaponID) const;
	bool AddWeapon(short weaponID);
	bool AddAmmo(int amount);

	void OnUpdate(float elapsedTime) override;
	void OnHit(int damage) override;

	void AddScore(int amount);
	int GetScore() const;

private:
	std::vector<Weapon*> availableWeapons;

	int score = 0;
};