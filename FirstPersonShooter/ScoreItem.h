#pragma once

#include "Item.h"

class ScoreItem : public Item
{
public:
	ScoreItem(EngineFPS* engine, int worth, Sprite* spr);

	void OnUse(Player& player) override;

protected:
	int worth;
};