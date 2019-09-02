#pragma once

#include "GameObject.h"

struct Weapon;

struct Item : GameObject
{
	Item(EngineFPS *engine);
	virtual ~Item();

	bool removed = false;

	virtual void OnUse(Player *player) = 0;
};

struct WeaponItem : Item
{
	WeaponItem(EngineFPS *engine, Weapon *weapon, Sprite *spr);

	Weapon *weapon;

	void OnUse(Player *player) override;
};

struct Medkit : Item
{
	Medkit(EngineFPS *engine, int healthBonus, Sprite *spr);

	int healthBonus;

	void OnUse(Player *player) override;
};