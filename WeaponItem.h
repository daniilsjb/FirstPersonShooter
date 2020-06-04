#pragma once

#include "Item.h"

class WeaponItem : public Item
{
public:
	WeaponItem(EngineFPS* engine, float x, float y, short weaponID, short ammoWorth, Sprite* spr);

	void OnUse(Player& player) override;

protected:
	short weaponID;
	short ammoWorth;
};