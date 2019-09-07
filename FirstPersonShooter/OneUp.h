#pragma once

#include "Item.h"

class OneUp : public Item
{
public:
	OneUp(EngineFPS* engine, float x, float y, Sprite* spr);

	void OnUse(Player& player) override;
};