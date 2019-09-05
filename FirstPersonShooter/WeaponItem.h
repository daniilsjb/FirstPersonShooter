#pragma once

#include "Item.h"

class WeaponItem : public Item
{
public:
	WeaponItem(EngineFPS* engine, short weaponID, short ammoWorth, Sprite* spr);

	void OnUse(Player* player) override;

protected:
	short weaponID;
	short ammoWorth;
};