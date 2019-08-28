#pragma once

#include "GameObject.h"

struct Item : GameObject
{
	Item(EngineFPS *engine);
	virtual ~Item();

	bool removed = false;

	virtual void OnUse(Player *player) = 0;
};

struct GunItem : Item
{
	GunItem(EngineFPS *engine);

	void OnUse(Player *player) override;
};

struct Medkit : Item
{
	Medkit(EngineFPS *engine, int healthBonus, Sprite *spr);

	int healthBonus;

	void OnUse(Player *player) override;
};