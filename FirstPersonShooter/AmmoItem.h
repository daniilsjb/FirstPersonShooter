#pragma once

#include "Item.h"

class AmmoItem : public Item
{
public:
	AmmoItem(EngineFPS* engine, int amount, Sprite* spr);

	void OnUse(Player& player) override;

protected:
	int weaponID;
	int amount;
};