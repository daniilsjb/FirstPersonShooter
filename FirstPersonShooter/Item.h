#pragma once

#include "GameObject.h"

class Weapon;

class Item : public GameObject
{
public:
	Item(EngineFPS *engine);
	virtual ~Item();

	bool IsRemoved() const;

	virtual void OnUse(Player *player) = 0;

protected:
	bool removed = false;
};

class WeaponItem : public Item
{
public:
	WeaponItem(EngineFPS *engine, Weapon *weapon, Sprite *spr);

	void OnUse(Player *player) override;

protected:
	Weapon *weapon;
};

class Medkit : public Item
{
public:
	Medkit(EngineFPS *engine, int healthBonus, Sprite *spr);

	void OnUse(Player *player) override;

protected:
	int healthBonus;
};