#include "AmmoItem.h"
#include "EngineFPS.h"
#include "Player.h"

AmmoItem::AmmoItem(EngineFPS *engine, short weaponID, int amount, Sprite *spr) : Item(engine), weaponID(weaponID), amount(amount)
{
	texture = spr;
}

void AmmoItem::OnUse(Player *player)
{
	if (player->AddAmmo(weaponID, amount))
	{
		removed = true;
		engine->PlayAudio("Ammo");
	}
}