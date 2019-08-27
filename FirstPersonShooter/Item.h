#pragma once

#include "GameObject.h"

struct Item : GameObject
{
	Item(EngineFPS *engine);
	virtual ~Item();

	bool removed = false;

	virtual void OnUse(Player *player) = 0;
};

struct Medpack : Item
{
	Medpack(EngineFPS *engine);

	int healthBonus = 10;

	void OnUse(Player *player) override;
};

struct Medkit : Item
{
	Medkit(EngineFPS *engine);

	int healthBonus = 40;

	void OnUse(Player *player) override;
};