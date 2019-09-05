#include "WeaponItem.h"
#include "EngineFPS.h"
#include "Player.h"

WeaponItem::WeaponItem(EngineFPS* engine, short weaponID, short ammoWorth, Sprite* spr) : Item(engine), weaponID(weaponID), ammoWorth(ammoWorth)
{
	texture = spr;
}

void WeaponItem::OnUse(Player *player)
{
	if (player->WeaponAcquired(weaponID))
	{
		if (player->AddAmmo(ammoWorth))
		{
			engine->PlayAudio("Ammo");
			removed = true;
		}
	}
	else if (player->AddWeapon(weaponID))
	{
		engine->PlayAudio("Ammo");
		removed = true;
	}
}