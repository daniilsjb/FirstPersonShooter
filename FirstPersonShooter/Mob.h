#pragma once

#include "DynamicObject.h"

class Weapon;

class Mob : public DynamicObject
{
public:
	Mob(EngineFPS *engine);
	virtual ~Mob();

	bool HasWeapon() const;
	Sprite* GetWeaponSprite() const;

	int GetWeaponAmmo() const;
	int GetWeaponCapacity() const;

	int GetMaxHealth() const;
	int GetHealth() const;

	void Heal(int amount);
	void Damage(int amount);

protected:
	Weapon *weapon = nullptr;

	int maxHealth;
	int currentHealth;
};