#include "WeaponItem.h"
#include "EngineFPS.h"
#include "Player.h"

WeaponItem::WeaponItem(EngineFPS* engine, short weaponID, Sprite* spr) : Item(engine), weaponID(weaponID)
{
	texture = spr;
}

void WeaponItem::OnUse(Player *player)
{
	if (player->AddWeapon(weaponID))
	{
		engine->PlayAudio("Ammo");
		removed = true;
	}
	else if (player->AddAmmoFromWeapon(weaponID))
	{
		engine->PlayAudio("Ammo");
		removed = true;
	}
}