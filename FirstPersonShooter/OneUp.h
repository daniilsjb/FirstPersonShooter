#pragma once

#include "Item.h"

class OneUp : public Item
{
public:
	OneUp(EngineFPS* engine, Sprite* spr);

	void OnUse(Player& player) override;
};