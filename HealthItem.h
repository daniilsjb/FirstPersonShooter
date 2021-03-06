#pragma once

#include "Item.h"

class HealthItem : public Item
{
public:
	HealthItem(EngineFPS* engine, float x, float y, int healthBonus, Sprite* spr);

	void OnUse(Player& player) override;

protected:
	int healthBonus;
};