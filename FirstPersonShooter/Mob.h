#pragma once

#include "DynamicObject.h"

class Weapon;

class Mob : public DynamicObject
{
public:
	Mob(EngineFPS* engine);
	virtual ~Mob();

	bool HasWeapon() const;
	Sprite* GetWeaponSprite() const;

	int GetWeaponAmmo() const;
	bool FullAmmo() const;
	void RestoreAmmo();

	int GetMaxHealth() const;
	int GetHealth() const;
	bool FullHealth() const;
	void RestoreHealth();

	void Heal(int amount);
	void Damage(int amount);

	bool IsDead() const;

protected:
	Weapon* weapon = nullptr;

	int maxHealth;
	int currentHealth;
};