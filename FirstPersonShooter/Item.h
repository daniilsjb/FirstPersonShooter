#pragma once

#include "GameObject.h"

class Weapon;

class Item : public GameObject
{
public:
	Item(EngineFPS *engine);
	virtual ~Item();

	bool removed = false;

	virtual void OnUse(Player *player) = 0;
};

class WeaponItem : public Item
{
public:
	WeaponItem(EngineFPS *engine, Weapon *weapon, Sprite *spr);

	Weapon *weapon;

	void OnUse(Player *player) override;
};

class Medkit : public Item
{
public:
	Medkit(EngineFPS *engine, int healthBonus, Sprite *spr);

	int healthBonus;

	void OnUse(Player *player) override;
};